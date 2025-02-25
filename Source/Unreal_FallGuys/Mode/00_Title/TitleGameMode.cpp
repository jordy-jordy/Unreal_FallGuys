// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitleGameMode.h"
#include "Net/UnrealNetwork.h"


ATitleGameMode::ATitleGameMode()
{
	ConnectedPlayers = 0;
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATitleGameMode::Tick(float _Delta)
{
	Super::Tick(_Delta);
}

void ATitleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority()) // 서버에서만 실행
	{
		ConnectedPlayers++;
		UE_LOG(LogTemp, Warning, TEXT("Server: Cur Player Number = %d"), ConnectedPlayers);
	}
}

void ATitleGameMode::OnRep_ConnectedPlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("Client: Cur Player Number = %d"), ConnectedPlayers);
}

void ATitleGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATitleGameMode, ConnectedPlayers);
}