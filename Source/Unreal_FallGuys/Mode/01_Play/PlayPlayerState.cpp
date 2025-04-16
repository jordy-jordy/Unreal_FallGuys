// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayPlayerState.h"

#include <Global/GlobalEnum.h>
#include <Global/BaseGameInstance.h>
#include <Mode/01_Play/PlayGameState.h>
#include "Mode/01_Play/PlayCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include <Net/UnrealNetwork.h>


APlayPlayerState::APlayPlayerState()
{
	PlayerStatus = PlayerInfo.Status;
}

void APlayPlayerState::BeginPlay()
{
	Super::BeginPlay();

	//CheckPlayer();
}

void APlayPlayerState::SetPlayerTag_Implementation(const FName& _Tag)
{
	// 유니크 아이디 설정
	PlayerInfo.UniqueID = GetUniqueId()->ToString();
    PlayerInfo.Tag = _Tag;
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



//LMH
// 실패한 플레이어 콜리전 처리
void APlayPlayerState::CheckPlayer()
{
	
	if (HasAuthority()) {
		S2M_CheckPlayer_Implementation();
	}
	else
	{
		C2S_CheckPlayer();
	}
}

void APlayPlayerState::C2S_CheckPlayer_Implementation()
{
	S2M_CheckPlayer_Implementation();
}

void APlayPlayerState::S2M_CheckPlayer_Implementation()
{
	if (Cast<APlayCharacter>(GetPawn()))
	{
		UBaseGameInstance* GameIns = GetPawn()->GetGameInstance<UBaseGameInstance>();
		if (GameIns == nullptr) return;
	
		if (false == GameIns->bIsResultLevel)
		{
			OutFailPlayer();
		}
		else
		{
			CheckFailPlayer();
		}

	}
}

void APlayPlayerState::CheckFailPlayer()
{

	if (EPlayerStatus::FAIL == PlayerInfo.Status)
	{
		bIsSpectar = true;
	}
	
}

void APlayPlayerState::OutFailPlayer()
{
	if (true == bIsSpectar)
	{
		GetPawn()->SetActorLocation({ 0,0,-1000000 });
		GetPawn()->SetActorEnableCollision(false);

		USkeletalMeshComponent* MeshComp = GetPawn()->FindComponentByClass<USkeletalMeshComponent>();

		if (MeshComp)
		{
			MeshComp->SetSimulatePhysics(false);
			MeshComp->SetEnableGravity(false);
		}

		// 다른 캐릭터로 시점 변경
	}
}

void APlayPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayPlayerState, PlayerInfo);
	DOREPLIFETIME(APlayPlayerState, PlayerStatus);
	DOREPLIFETIME(APlayPlayerState, PlayerDropOrder);
	DOREPLIFETIME(APlayPlayerState, bIsWinner);
}

