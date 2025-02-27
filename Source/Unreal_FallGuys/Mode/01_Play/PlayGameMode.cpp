// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h" 
#include <Unreal_FallGuys.h>

APlayGameMode::APlayGameMode()
{
	ConnectedPlayers = 0;
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




