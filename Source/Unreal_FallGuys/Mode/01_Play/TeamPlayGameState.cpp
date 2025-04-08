// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/TeamPlayGameState.h"
#include "Level/01_Play/Actor/EggSpawnManager.h"
#include "Net/UnrealNetwork.h"
#include "TeamPlayGameState.h"


void ATeamPlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATeamPlayGameState, GameStateREDTeamScore);
	DOREPLIFETIME(ATeamPlayGameState, GameStateBLUETeamScore);
	DOREPLIFETIME(ATeamPlayGameState, STATE_StageLimitTime);
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


