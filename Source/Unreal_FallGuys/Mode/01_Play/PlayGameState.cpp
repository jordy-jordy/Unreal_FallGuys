// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameState.h"

#include <Net/UnrealNetwork.h>
#include <Unreal_FallGuys.h>


APlayGameState::APlayGameState()
{
}

void APlayGameState::SyncPlayerInfoFromPlayerState_Implementation()
{
    PlayerInfoArray.Empty();

    for (APlayerState* PlayerState : PlayerArray)
    {
        APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayerState);
        if (PlayPlayerState)
        {
            PlayerInfoArray.Add(FPlayerInfoEntry(PlayPlayerState->PlayerInfo.UniqueID, PlayPlayerState->PlayerInfo));
            UE_LOG(FALL_DEV_LOG, Log, TEXT("GameState: 플레이어 정보 동기화 - UniqueId: %s, Tag: %s"),
                *PlayPlayerState->PlayerInfo.UniqueID, *PlayPlayerState->PlayerInfo.Tag);
        }
    }
}

void APlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameState, PlayerInfoArray);
}
