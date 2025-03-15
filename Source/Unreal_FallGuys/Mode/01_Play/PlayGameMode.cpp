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

	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (GameInstance)
	{
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

    if (!HasAuthority()) return;

    APlayPlayerState* PlayerState = Cast<APlayPlayerState>(NewPlayer->PlayerState);
    if (!PlayerState)
    {
        UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayerState is nullptr!"));
        return;
    }

    // 새로운 플레이어 등록
    APlayGameState* FallState = GetGameState<APlayGameState>();
    if (!FallState)
    {
        UE_LOG(FALL_DEV_LOG, Error, TEXT("GameState is nullptr!"));
        return;
    }

    FString UniqueTag = FString::Printf(TEXT("Player%d"), FallState->PlayerInfoArray.Num());
    PlayerState->SetPlayerInfo(UniqueTag, EPlayerStatus::DEFAULT);

    UE_LOG(FALL_DEV_LOG, Log, TEXT("서버: 신규 플레이어 태그 부여 - Controller = %s, Tag = %s"),
        *NewPlayer->GetName(), *UniqueTag);

    // 모든 클라이언트에게 정보 동기화
    SyncPlayerInfo(NewPlayer);

	ConnectedPlayers++;

    if (IsMinPlayersReached())
    {
        UE_LOG(FALL_DEV_LOG, Warning, TEXT("최소 인원 충족, 게임 시작 가능"));
        StartGame();
    }
}

// 플레이어 인포 동기화
void APlayGameMode::SyncPlayerInfo_Implementation(APlayerController* _NewPlayer)
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GameState is nullptr!"));
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
