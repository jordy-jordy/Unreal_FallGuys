// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h" 

#include <Unreal_FallGuys.h>
#include <Global/FallConst.h>


APlayGameMode::APlayGameMode()
{
	ConnectedPlayers = 0;
}

void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) // 서버에서만 실행
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("Server: PlayGameMode Started!"));
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("Client: PlayGameMode Started!"));
	}
}

void APlayGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (HasAuthority()) // 서버에서만 실행
	{
		ConnectedPlayers++;
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("Join To %s"), *CurrentLevelName);
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("Server: Player Joined, Cur Player Number = %d"), ConnectedPlayers);
		// 네트워크 동기화를 강제 실행하여 클라이언트와 데이터 맞추기
		ForceNetUpdate();
		//OnRep_ConnectedPlayers();
	}

	if (IsMinPlayersReached())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("MIN PLAYER READY. TRY GAME START"));
		StartGame();
	}
}

void APlayGameMode::OnRep_ConnectedPlayers()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("Client: Player Number Synced = %d"), ConnectedPlayers);
}

void APlayGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameMode, ConnectedPlayers);
}

bool APlayGameMode::IsMinPlayersReached()
{
	return ConnectedPlayers >= UFallConst::MinPlayerCount;
}

void APlayGameMode::StartGame()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("GAME START READY"));
	// 여기에 실제 게임 시작 로직 추가 예정
}



