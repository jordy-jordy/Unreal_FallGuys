// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayScoreWidget.h"
#include "Mode/01_Play/TeamPlayGameState.h"


void UPlayScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

int UPlayScoreWidget::GetTimeLimit_Minutes()
{
	ATeamPlayGameState* TeamPlayGameState = GetWorld()->GetGameState<ATeamPlayGameState>();
	float LimitTime = TeamPlayGameState->GetStageLimitTime();

	int Minutes = static_cast<int>(LimitTime / 60);

	return Minutes;
}

int UPlayScoreWidget::GetTimeLimit_Seconds()
{
	ATeamPlayGameState* TeamPlayGameState = GetWorld()->GetGameState<ATeamPlayGameState>();
	float LimitTime = TeamPlayGameState->GetStageLimitTime();

	int Seconds = static_cast<int>(LimitTime) % 60;

	return Seconds;
}

float UPlayScoreWidget::GetLimitTime()
{
	ATeamPlayGameState* TeamPlayGameState = GetWorld()->GetGameState<ATeamPlayGameState>();
	return TeamPlayGameState->GetStageLimitTime();
}
