// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayPlayerState.h"

#include <Net/UnrealNetwork.h>


APlayPlayerState::APlayPlayerState()
{
}

void APlayPlayerState::SetPlayerInfo_Implementation(const FString& _Tag, const FString& _NickName)
{
    PlayerInfo.UniqueID = GetUniqueId()->ToString();
    PlayerInfo.Tag = _Tag;
    PlayerInfo.NickName = _NickName;
}

void APlayPlayerState::SetTeam_Implementation(ETeamType _Team)
{
    PlayerInfo.Team = _Team;
}

void APlayPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APlayPlayerState, PlayerInfo);
}

// 플레이어 상태 전환 : 성공 (서버)
void APlayPlayerState::S2M_SetPlayerStatusSuccess_Implementation()
{
	PlayerInfo.Status = EPlayerStatus::SUCCESS;
}

// 플레이어 상태 전환 : 실패 (서버)
void APlayPlayerState::S2M_SetPlayerStatusFail_Implementation()
{
	PlayerInfo.Status = EPlayerStatus::FAIL;
}

// 플레이어 상태 전환 : 디폴트 (서버)
void APlayPlayerState::S2M_SetPlayerStatusDefault_Implementation()
{
	PlayerInfo.Status = EPlayerStatus::DEFAULT;
}