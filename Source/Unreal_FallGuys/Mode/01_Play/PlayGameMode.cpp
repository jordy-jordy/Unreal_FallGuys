// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h" 

#include <Unreal_FallGuys.h>
#include <Global/FallConst.h>
#include <Global/BaseGameInstance.h>
#include <Mode/01_Play/PlayGameState.h>
#include <Mode/01_Play/PlayPlayerState.h>


APlayGameMode::APlayGameMode()
{
	ConnectedPlayers = 0;
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
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 기존 플레이어 감지. 정보를 로드합니다."));

		FPlayerInfo RestoredInfo;
		if (GameInstance->InsGetBackedUpPlayerInfo(PlayerUniqueID, RestoredInfo))
		{
			RestoredInfo.Status = EPlayerStatus::DEFAULT;  // Status 초기화
			PlayerState->PlayerInfo = RestoredInfo;

			UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 플레이어 정보 로드 완료 - UniqueID = %s, Tag = %s"),
				*RestoredInfo.UniqueID, *RestoredInfo.Tag);
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 신규 플레이어 감지. 정보를 세팅합니다."));

		// 새로운 Player 등록 및 세팅
		FString UniqueTag = FString::Printf(TEXT("Player%d"), FallState->PlayerInfoArray.Num());
		PlayerState->SetPlayerInfo(UniqueTag, EPlayerStatus::DEFAULT);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 신규 플레이어 정보 세팅 - UniqueID = %s, Tag = %s"),
			*PlayerState->PlayerInfo.UniqueID, *UniqueTag);
	}

	// 접속 여부 bool값 true로 변경
	GameInstance->SetbIsConnectedTrue();

    // 모든 클라이언트에게 정보 동기화
    SyncPlayerInfo();

	if (!GameInstance->IsMovedLevel)
	{
		// 접속중인 Player 수 증가
		ConnectedPlayers++;
	}

    if (IsMinPlayersReached())
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

// 게임 시작
void APlayGameMode::StartGame_Implementation()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("게임이 시작되었습니다."));
	UFallConst::CanStart = true;
}

void APlayGameMode::OnRep_ConnectedPlayers()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("클라이언트: ConnectedPlayers 동기화 = %d"), ConnectedPlayers);
}

// 동기화 변수
void APlayGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameMode, ConnectedPlayers);
}

