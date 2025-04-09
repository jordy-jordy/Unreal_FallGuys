// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayClearCountWidget.h"
#include "Global/FallGlobal.h"
#include "Mode/01_Play/PlayGameState.h"


void UPlayClearCountWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

int UPlayClearCountWidget::GetWholePlayerNum()
{
	APlayGameState* PlayGameState = Cast<APlayGameState>(GetWorld()->GetGameState());

	if (nullptr == PlayGameState)
	{
		return 0;
	}

	if (true == UFallGlobal::GetSettedGoalCountDone())
	{
		return PlayGameState->GetGameStateFinishPlayer();
	}

	return 0;
}

int UPlayClearCountWidget::GetTargetPlayerNum()
{
	APlayGameState* PlayGameState = Cast<APlayGameState>(GetWorld()->GetGameState());

	if (nullptr == PlayGameState)
	{
		return 0;
	}

	// �������ӷ����� ���� �޶�������?
	return PlayGameState->GetGameStateCurFinishPlayer();
}

