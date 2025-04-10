// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/TeamPlayGameState.h"

#include "Net/UnrealNetwork.h"
#include "Unreal_FallGuys.h"

#include "Level/01_Play/Actor/EggSpawnManager.h"


void ATeamPlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATeamPlayGameState, GameStateREDTeamScore);
	DOREPLIFETIME(ATeamPlayGameState, GameStateBLUETeamScore);
	DOREPLIFETIME(ATeamPlayGameState, STATE_StageLimitTime);
	DOREPLIFETIME(ATeamPlayGameState, STATE_RemainingTime);
}

void ATeamPlayGameState::SetREDTeamScore_Implementation(int32 _Score)
{
	GameStateREDTeamScore = _Score;
}

void ATeamPlayGameState::SetBLUETeamScore_Implementation(int32 _Score)
{
	GameStateBLUETeamScore = _Score;
}

void ATeamPlayGameState::SetStageLimitTime_Implementation(float _Time)
{
	STATE_StageLimitTime = _Time;
}

void ATeamPlayGameState::SetRemainingTime_Implementation(float _Time)
{
	STATE_RemainingTime = _Time;
}

void ATeamPlayGameState::OnRep_RemainingTime()
{
	// 클라이언트에서 남은 시간이 갱신됐을 때 실행되는 콜백
	// 필요하면 여기서 UI 이벤트 호출 가능 (예: 위젯에 바인딩)
	UE_LOG(FALL_DEV_LOG, Log, TEXT("TeamPlayGameState :: Client :: 남은 제한 시간 갱신: %.2f"), STATE_RemainingTime);
}

bool ATeamPlayGameState::SetRemainingTime_Validate(float _Time)
{
	return true;
}

// LMH
void ATeamPlayGameState::SpawnEggManager()
{
	if (HasAuthority())
	{
		Spawnner = GetWorld()->SpawnActor<AEggSpawnManager>(SpawnManagerFactory, FVector(0, 0, 400.0f), FRotator(0, 0, 0));
		if (Spawnner == nullptr) return;
		Spawnner->SetOwner(this);
	}
}

TArray<int> ATeamPlayGameState::GetTeamEggCount() const
{
	return Spawnner->GetTeamEggCount();
}


