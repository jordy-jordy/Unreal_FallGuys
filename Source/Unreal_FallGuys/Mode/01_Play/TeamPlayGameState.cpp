// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/TeamPlayGameState.h"
#include "Net/UnrealNetwork.h"


void ATeamPlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATeamPlayGameState, GameStateREDTeamScore);
	DOREPLIFETIME(ATeamPlayGameState, GameStateBLUETeamScore);
	DOREPLIFETIME(ATeamPlayGameState, GameStateREDTeamEggCount);
	DOREPLIFETIME(ATeamPlayGameState, GameStateBLUETeamEggCount);
}

void ATeamPlayGameState::SetREDTeamScore_Implementation(int32 _Score)
{

}

void ATeamPlayGameState::SetBLUETeamScore_Implementation(int32 _Score)
{

}
