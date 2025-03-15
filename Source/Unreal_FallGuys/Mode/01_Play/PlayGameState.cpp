// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameState.h"

#include <Net/UnrealNetwork.h>
#include <Unreal_FallGuys.h>
#include <Mode/01_Play/PlayPlayerState.h>


APlayGameState::APlayGameState()
{
}

void APlayGameState::SyncPlayerInfoFromPlayerState()
{
	PlayerInfoArray.Empty();

	for (APlayerState* PlayerState : PlayerArray)
	{
		APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayerState);
		if (PlayPlayerState)
		{
			APlayerController* Controller = Cast<APlayerController>(PlayPlayerState->GetOwner());
			PlayerInfoArray.Add(FPlayerInfoEntry(Controller, PlayPlayerState->PlayerInfo));

			UE_LOG(FALL_DEV_LOG, Log, TEXT("GameState: 플레이어 정보 동기화 - Controller: %s, UniqueId: %s, Tag: %s"),
				*Controller->GetName(), *PlayPlayerState->PlayerInfo.UniqueID, *PlayPlayerState->PlayerInfo.Tag);
		}
	}
}

void APlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameState, PlayerInfoArray);
}
