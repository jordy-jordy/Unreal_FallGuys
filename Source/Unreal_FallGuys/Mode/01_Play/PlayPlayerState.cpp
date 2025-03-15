// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayPlayerState.h"

#include <Net/UnrealNetwork.h>


APlayPlayerState::APlayPlayerState()
{
}

void APlayPlayerState::SetPlayerInfo(const FString& _Tag, EPlayerStatus _Status)
{
    PlayerInfo.Tag = _Tag;
    PlayerInfo.Status = _Status;
    PlayerInfo.UniqueID = GetUniqueId()->ToString();
}

void APlayPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APlayPlayerState, PlayerInfo);
}
