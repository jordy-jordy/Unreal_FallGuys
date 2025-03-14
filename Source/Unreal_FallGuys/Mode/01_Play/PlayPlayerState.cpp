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

    // UniqueId가 없을 경우 새로 설정
    if (PlayerUniqueId.IsEmpty() && GetUniqueId().IsValid())
    {
        PlayerUniqueId = GetUniqueId()->ToString();
    }
}

void APlayPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APlayPlayerState, PlayerInfo);
    DOREPLIFETIME(APlayPlayerState, PlayerUniqueId);
}
