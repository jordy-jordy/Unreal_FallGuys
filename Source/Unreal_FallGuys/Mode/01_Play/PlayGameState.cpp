// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameState.h"

#include <Net/UnrealNetwork.h>
#include <Unreal_FallGuys.h>


APlayGameState::APlayGameState()
{
}

void APlayGameState::SyncPlayerInfoFromPlayerState_Implementation()
{
	TMap<FString, FPlayerInfo> TempMap;
	for (APlayerState* PlayerState : PlayerArray)
	{
		APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayerState);
		if (PlayPlayerState)
		{
			TempMap.Add(PlayPlayerState->PlayerInfo.UniqueID, PlayPlayerState->PlayerInfo);
		}
	}

	PlayerInfoArray.Empty();
	for (const auto& Elem : TempMap)
	{
		PlayerInfoArray.Add(FPlayerInfoEntry(Elem.Key, Elem.Value));
		UE_LOG(FALL_DEV_LOG, Log, TEXT("GameState: 플레이어 정보 동기화 - UniqueId: %s, Tag: %s"),
			*Elem.Key, *Elem.Value.Tag);
	}
}

void APlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameState, PlayerInfoArray);
}
