// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayPlayerState.h"

#include <Global/GlobalEnum.h>

#include <Net/UnrealNetwork.h>


APlayPlayerState::APlayPlayerState()
{
}

void APlayPlayerState::SetPlayerInfo_Implementation(const FName& _Tag, const FString& _NickName)
{
    PlayerInfo.UniqueID = GetUniqueId()->ToString();
    PlayerInfo.Tag = _Tag;
    PlayerInfo.NickName = _NickName;
}

void APlayPlayerState::SetTeam_Implementation(ETeamType _Team)
{
    PlayerInfo.Team = _Team;
}

// 플레이어 상태 설정
void APlayPlayerState::SetPlayerStatus_Implementation(EPlayerStatus _NewStatus)
{
	if (!HasAuthority()) return; // 서버에서만 실행

	// 상태가 다르면 변경
	if (PlayerStatus != _NewStatus)
	{
		PlayerStatus = _NewStatus;

		// 구조체에도 반영
		PlayerInfo.Status = _NewStatus;
	}
}

// PlayerStatus 가 변할 때 호출되는 함수 - 동기화
void APlayPlayerState::OnRep_PlayerStatus()
{
	// 구조체 동기화
	PlayerInfo.Status = PlayerStatus;
}

// 플레이어 떨어지는 순서 설정
void APlayPlayerState::SetPlayerDropOrder_Implementation(int32 _Order)
{
	if (!HasAuthority()) return; // 서버에서만 실행

	// 상태가 다르면 변경
	if (PlayerDropOrder != _Order)
	{
		PlayerDropOrder = _Order;

		// 구조체에도 반영
		PlayerInfo.DropOrder = _Order;
	}
}

// PlayerDropOrder 가 변할 때 호출되는 함수 - 동기화
void APlayPlayerState::OnRep_PlayerDropOrder()
{
	// 구조체 동기화
	PlayerInfo.DropOrder = PlayerDropOrder;
}

void APlayPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayPlayerState, PlayerInfo);
	DOREPLIFETIME(APlayPlayerState, PlayerStatus);
	DOREPLIFETIME(APlayPlayerState, PlayerDropOrder);
}

