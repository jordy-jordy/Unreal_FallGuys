// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameMode.h"
#include "Kismet/GameplayStatics.h" 
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

#include <Unreal_FallGuys.h>
#include <Global/FallConst.h>
#include <Global/BaseGameInstance.h>
#include <Mode/01_Play/PlayGameState.h>
#include <Mode/01_Play/PlayPlayerState.h>
#include <Mode/01_Play/PlayCharacter.h>


APlayGameMode::APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) // 서버에서만 실행
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("서버: PlayGameMode가 시작되었습니다."));
	}
}

void APlayGameMode::ServerTravelToNextMap(const FString& url)
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT(":: 서버트래블 감지 ::"));

	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		// 현재 게임 상태 가져오기
		APlayGameState* PlayGameState = GetGameState<APlayGameState>();
		if (PlayGameState)
		{
			for (const FPlayerInfoEntry& PlayerEntry : PlayGameState->PlayerInfoArray)
			{
				GameInstance->InsBackupPlayerInfo(PlayerEntry.UniqueID, PlayerEntry.PlayerInfo);
				UE_LOG(FALL_DEV_LOG, Log, TEXT("ServerTravelToNextMap :: 플레이어 정보 백업 완료 - UniqueID = %s, Tag = %s"),
					*PlayerEntry.UniqueID, *PlayerEntry.PlayerInfo.Tag);
			}
		}

		GameInstance->IsMovedLevel = true;
	}

	GetWorld()->ServerTravel(url, false);
}

void APlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 게임 종료 체크
	if (true == IsEndGame) return;

	// 골인한 플레이어 수와 목표 인원 수 체크
	if (CurFinishPlayer >= FinishPlayer)
	{
		IsEndGame = true;

		ServerTravelToNextMap(NextLevel);
	}

}

// 접속시 실행되는 함수
void APlayGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

	// 서버장이 아닐시 리턴
    if (!HasAuthority()) return;

	// 현재 접속한 플레이어 수가 최소 인원 이상이면 접속 거부
	if (ConnectedPlayers >= UFallConst::MinPlayerCount)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("접속 거부: 현재 플레이어 수(%d)가 제한(%d) 이상입니다."), ConnectedPlayers, UFallConst::MinPlayerCount);

		// 클라이언트를 강제 종료
		NewPlayer->ClientTravel(TEXT("/Game/Maps/TitleLevel"), TRAVEL_Absolute);
		return;
	}

	// PlayerState가 없을시 리턴
    APlayPlayerState* PlayerState = Cast<APlayPlayerState>(NewPlayer->PlayerState);
    if (!PlayerState)
    {
        UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayerState가 nullptr 입니다."));
        return;
    }

    // GameState가 없을시 리턴
    APlayGameState* FallState = GetGameState<APlayGameState>();
    if (!FallState)
    {
        UE_LOG(FALL_DEV_LOG, Error, TEXT("GameState가 nullptr 입니다."));
        return;
    }

	// 기존 Player 정보 백업
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	FString PlayerUniqueID = PlayerState->GetUniqueId()->ToString(); // UniqueID 얻음

	if (GameInstance && GameInstance->IsMovedLevel)
	{
		// 스테이지 전환 되었을때 IsDie를 true로 초기화
		GameInstance->SetIsDie(true);

		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 기존 플레이어 감지. 정보를 로드합니다."));

		FPlayerInfo RestoredInfo;
		if (GameInstance->InsGetBackedUpPlayerInfo(PlayerUniqueID, RestoredInfo))
		{
			RestoredInfo.Status = EPlayerStatus::DEFAULT;  // Status 초기화
			PlayerState->PlayerInfo = RestoredInfo;

			// 태그 복구
			NewPlayer->Tags.Add(*RestoredInfo.Tag);
			// 안전하게 태그 값 가져오기
			FString TagString = (NewPlayer->Tags.Num() > 0) ? NewPlayer->Tags[0].ToString() : TEXT("태그 없음");

			UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 플레이어 정보 로드 완료 - UniqueID = %s, Tag = %s"),
				*RestoredInfo.UniqueID, *TagString);
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 신규 플레이어 감지. 정보를 세팅합니다."));

		// 새로운 Player 등록 및 세팅
		FString UniqueTag = FString::Printf(TEXT("Player%d"), FallState->PlayerInfoArray.Num());
		PlayerState->SetPlayerInfo(UniqueTag, EPlayerStatus::DEFAULT);
		
		// 태그 부여
		NewPlayer->Tags.Add(*UniqueTag);
		// 안전하게 태그 값 가져오기
		FString TagString = (NewPlayer->Tags.Num() > 0) ? NewPlayer->Tags[0].ToString() : TEXT("태그 없음");

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 신규 플레이어 정보 세팅 - UniqueID = %s, Tag = %s"),
			*PlayerState->PlayerInfo.UniqueID, *TagString);
	}

	// 접속 여부 bool값 true로 변경
	GameInstance->InsSetbIsConnectedTrue();

    // 모든 클라이언트에게 정보 동기화
    SyncPlayerInfo();

	if (!GameInstance->IsMovedLevel)
	{
		// 접속중인 Player 수 증가
		ConnectedPlayers++;
	}

    if (GameInstance->IsMovedLevel || IsMinPlayersReached())
    {
        UE_LOG(FALL_DEV_LOG, Warning, TEXT("최소 인원 충족, 게임 시작 가능"));
        StartGame();
    }
}

// 플레이어 인포 동기화
void APlayGameMode::SyncPlayerInfo_Implementation()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GameState가 nullptr 입니다. 일정 시간 후 다시 시도합니다."));
		return;
	}

	FallState->SyncPlayerInfoFromPlayerState();
}

// 최소 인원 체크
bool APlayGameMode::IsMinPlayersReached()
{
	return ConnectedPlayers >= UFallConst::MinPlayerCount;
}

// 캐릭터 이동 가능하게 세팅
void APlayGameMode::SetCharacterMovePossible_Implementation(APlayCharacter* _Player)
{
	_Player->S2M_SetCanMoveTrue();
}

// 게임 시작
void APlayGameMode::StartGame_Implementation()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("게임을 시작합니다."));
	StartCountdownTimer();
}

// 카운트 다운 핸들 시작
void APlayGameMode::StartCountdownTimer_Implementation()
{
	if (!HasAuthority() || !GetWorld()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("카운트다운 시작, 3초 대기"));

	// 3초 후 카운트다운 시작
	FTimerHandle DelayTimer;
	GetWorldTimerManager().SetTimer(DelayTimer, this, &APlayGameMode::StartCountdown, 0.5f, false);
}

// 카운트다운 시작 (3초 대기 후 실행)
void APlayGameMode::StartCountdown()
{
	if (!HasAuthority() || !GetWorld()) return;

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("StartCountdown: GameState가 존재하지 않습니다!"));
		return;
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("카운트다운 진행 시작. 초기 값: %.0f"), FallState->CountDownTime);

	if (FallState->CountDownTime <= 0)
	{
		FallState->CountDownTime = 10.0f;
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("카운트다운 값이 0이거나 음수라 기본값(10초)으로 설정"));
	}

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &APlayGameMode::UpdateCountdown, 1.0f, true);
}

// 카운트다운 진행 (매초 실행)
void APlayGameMode::UpdateCountdown()
{
	if (!HasAuthority()) return;

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	FallState->CountDownTime -= 1.0f;
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("카운트다운: %.0f"), FallState->CountDownTime);

	if (FallState->CountDownTime <= 0.0f)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("카운트다운 종료, 캐릭터 이동 가능"));

		for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
		{
			APlayCharacter* PlayerCharacter = *It;
			if (PlayerCharacter)
			{
				SetCharacterMovePossible(PlayerCharacter);
			}
		}
	}
}

// 동기화 변수
void APlayGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
