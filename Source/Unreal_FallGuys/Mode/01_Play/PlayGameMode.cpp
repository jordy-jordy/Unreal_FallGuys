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

	if (HasAuthority())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode BeginPlay START ======= "));
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode BeginPlay END ======= "));
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

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin START ======= "));

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

	// 접속 제한을 사용하는 경우 인원 체크 및 접속 거부 실행
	if (true == UFallConst::UsePlayerLimit)
	{
		int ConnectingPlayer = FallState->GetConnectedPlayers();
		// 현재 접속한 플레이어 수가 최소 인원 이상이면 접속 거부
		if (ConnectingPlayer >= UFallConst::MinPlayerCount)
		{
			UE_LOG(FALL_DEV_LOG, Error, TEXT("접속 거부: 현재 플레이어 수(%d)가 제한(%d) 이상입니다."), ConnectingPlayer, UFallConst::MinPlayerCount);

			// 클라이언트를 강제 종료
			NewPlayer->ClientTravel(TEXT("/Game/Maps/TitleLevel"), TRAVEL_Absolute);
			return;
		}
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
		FallState->AddConnectedPlayers();
		int ConnectingPlayer = FallState->GetConnectedPlayers();
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 접속자 수 : %d"), ConnectingPlayer);
	}

	// 게임 인스턴스에 저장된 레벨 이름을 게임 스테이트에 저장
	FallState->SavePlayLevelName(GameInstance->InsGetCurLevelName());
	// 게임 인스턴스에 저장된 레벨 에셋 이름을 게임 스테이트에 저장
	FallState->SavePlayLevelAssetName(GameInstance->InsGetCurLevelAssetName());

	// 게임 시작 조건 핸들
	HandleStartConditions();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin END ======= "));
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
	APlayGameState* FallState = GetGameState<APlayGameState>();
	return FallState->GetConnectedPlayers() >= UFallConst::MinPlayerCount;
}

// 캐릭터 이동 가능하게 세팅
void APlayGameMode::SetCharacterMovePossible_Implementation(APlayCharacter* _Player)
{
	_Player->S2M_SetCanMoveTrue();
}

// 게임 시작 조건 처리
void APlayGameMode::HandleStartConditions()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());

	if (!FallState || !GameInstance)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("HandleStartConditions: GameState 또는 GameInstance가 nullptr입니다."));
		return;
	}

	// 카운트 다운을 사용하지 않는 경우 → 바로 이동 가능
	if (!UFallConst::UseCountDown)
	{
		for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
		{
			APlayCharacter* PlayerCharacter = *It;
			if (PlayerCharacter)
			{
				SetCharacterMovePossible(PlayerCharacter);
			}
		}
		FallState->IsCountDownOver = true;
		UE_LOG(FALL_DEV_LOG, Log, TEXT("카운트 다운 사용 안함 - 바로 이동 가능"));
	}
	else if (GameInstance->IsMovedLevel || IsMinPlayersReached())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("게임 시작을 위한 최소 인원이 모였습니다. 게임이 시작됩니다."));
		StartGame();
	}
}

// 게임 시작
void APlayGameMode::StartGame_Implementation()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("게임을 시작합니다."));

	StartCountdownTimer();
}

// 게임 시작 전 카운트다운 핸들 활성화
void APlayGameMode::StartCountdownTimer_Implementation()
{
	if (!HasAuthority() || !GetWorld()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("카운트다운 시작, 0.5초 대기"));

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

		// 카운트 다운 끝났음을 알림
		FallState->IsCountDownOver = true;
	}
}

// 스테이지 제한 시간 타이머 활성화
void APlayGameMode::StartStageLimitTimer_Implementation()
{
	if (!HasAuthority()) return;

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	if (FallState->UseStageLimitTime == false)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("스테이지 제한 시간을 사용하지 않으므로 타이머 시작 안함"));
		return;
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("스테이지 제한 시간 타이머 시작: %.2f초"), FallState->StageLimitTime);

	GetWorldTimerManager().SetTimer(StageLimitTimerHandle, this, &APlayGameMode::OnStageLimitTimeOver, FallState->StageLimitTime, false);
}

// 스테이지 제한 시간 오버 처리
void APlayGameMode::OnStageLimitTimeOver()
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("제한 시간 초과! 스테이지를 종료합니다."));

	// 예시: 남은 플레이어 FAIL 처리
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->PlayerInfo.Status == EPlayerStatus::DEFAULT)
		{
			PState->PlayerInfo.Status = EPlayerStatus::FAIL;
			UE_LOG(FALL_DEV_LOG, Log, TEXT("Player FAIL 처리됨: %s"), *PState->PlayerInfo.Tag);
		}
	}

	// 다음 맵 이동
	ServerTravelToNextMap(NextLevel);
}

// 동기화 변수
void APlayGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
