// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayClearCountWidget.h"
#include "Mode/01_Play/PlayGameState.h"


int UPlayClearCountWidget::GetWholePlayerNum()
{
	APlayGameState* PlayGameState = Cast<APlayGameState>(GetWorld()->GetGameState());

	if (nullptr == PlayGameState)
	{
		return 0;
	}

	return PlayGameState->GetGameStateFinishPlayer();
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