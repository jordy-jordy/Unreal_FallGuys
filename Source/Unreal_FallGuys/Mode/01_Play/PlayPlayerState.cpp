// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayPlayerState.h"
#include <GameFramework/PawnMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Net/UnrealNetwork.h>

#include <Unreal_FallGuys.h>
#include <Global/GlobalEnum.h>
#include <Global/BaseGameInstance.h>
#include <Mode/01_Play/PlayGameState.h>
#include <Mode/01_Play/PlayCharacter.h>


APlayPlayerState::APlayPlayerState()
{
	PlayerStatus = PlayerInfo.Status;
}

void APlayPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void APlayPlayerState::SetPlayerTag_Implementation(const FName& _Tag)
{
	// 플레이어 태그 설정
    PlayerInfo.Tag = _Tag;

	// 유니크 아이디 설정
	PlayerInfo.UniqueID = GetUniqueId()->ToString();
}

void APlayPlayerState::OnRep_PlayerInfo()
{
	APlayCharacter* MyCharacter = Cast<APlayCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->InitializeFromPlayerInfo(PlayerInfo);  // 여기서 직접 캐릭터 세팅
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("OnRep_PlayerInfo :: 복제 완료 - 태그: %s"), *PlayerInfo.Tag.ToString());
}

// 클라들에게도 플레이어 인포 동기화
void APlayPlayerState::MCAST_ApplyPlayerInfo_Implementation(const FPlayerInfo& _Info)
{
	PlayerInfo.NickName = _Info.NickName;
	PlayerInfo.CostumeColor = _Info.CostumeColor;
	PlayerInfo.CostumeTOP = _Info.CostumeTOP;
	PlayerInfo.CostumeBOT = _Info.CostumeBOT;
}

void APlayPlayerState::SetTeam_Implementation(ETeamType _Team)
{
    PlayerInfo.Team = _Team;
}

// 플레이어 상태 설정 : 게임 끝나기 전
void APlayPlayerState::SetPlayerStatus_Implementation(EPlayerStatus _NewStatus)
{
	if (!HasAuthority()) return; // 서버에서만 실행

	// 이미 실패한 플레이어면 패스
	if (PlayerStatus == EPlayerStatus::FAIL) return;
	if (PlayerInfo.Status == EPlayerStatus::FAIL) return;

	// 상태가 다르면 변경
	if (PlayerStatus != _NewStatus)
	{
		PlayerStatus = _NewStatus;

		// 구조체에도 반영
		PlayerInfo.Status = _NewStatus;
	}
}

// 플레이어 상태 설정 : 게임 끝난 후 일괄 변경
void APlayPlayerState::SetPlayerStatusOnEnd_Implementation(EPlayerStatus _NewStatus)
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

// 승자 표시 함수
void APlayPlayerState::SetIsWinner_Implementation(bool _bWinner)
{
	bIsWinner = _bWinner;
}

// 컨트롤러 및 캐릭터의 준비 상태 세팅
void APlayPlayerState::SetControllerReadyToGame()
{
	bControllerReadyToGame = true;
}

// 결과 화면인지 세팅 : PlayGameMode에서 호출
void APlayPlayerState::SetIsResultLevel_Implementation(bool _Value)
{
	bIsResultLevel = _Value;
}

// 관전자 세팅
void APlayPlayerState::SetPlayertoSpectar_Implementation(bool _Value)
{
	PlayerInfo.bIsSpectar = _Value;
}

// 결과 화면에서 가릴게
void APlayPlayerState::SetCanHiddenAtResult_Implementation(bool _Value)
{
	PlayerInfo.bCanHiddenAtResult = _Value;
}

// 플레이어 인포 동기화
void APlayPlayerState::S2M_SetPlayInfo_Implementation(FPlayerInfo _PlayerInfo)
{
	PlayerInfo = _PlayerInfo;
}

void APlayPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayPlayerState, PlayerInfo);
	DOREPLIFETIME(APlayPlayerState, PlayerStatus);
	DOREPLIFETIME(APlayPlayerState, PlayerDropOrder);
	DOREPLIFETIME(APlayPlayerState, bIsWinner);
	DOREPLIFETIME(APlayPlayerState, bIsResultLevel);
}

