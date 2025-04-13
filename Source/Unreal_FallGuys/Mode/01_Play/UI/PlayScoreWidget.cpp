// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayScoreWidget.h"
#include "Mode/01_Play/TeamPlayGameState.h"
#include "TimerManager.h"
#include "Global/FallGlobal.h"


void UPlayScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FString UPlayScoreWidget::GetTimeLimit_Minutes()
{
	int CurLimitTime = static_cast<int>(UFallGlobal::GetRemainingTime());
	int Minutes = static_cast<int>(CurLimitTime / 60);
	FString MText;

	if (Minutes < 10)
	{
		MText = TEXT("0") + FString::FromInt(Minutes);
	}
	else if (Minutes >= 10)
	{
		MText = FString::FromInt(Minutes);
	}

	return MText;
}

FString UPlayScoreWidget::GetTimeLimit_Seconds()
{
	int CurLimitTime = static_cast<int>(UFallGlobal::GetRemainingTime());
	int Seconds = static_cast<int>(CurLimitTime) % 60;
	FString SText;

	if (Seconds < 10)
	{
		SText = TEXT("0") + FString::FromInt(Seconds);
	}
	else if (Seconds >= 10)
	{
		SText = FString::FromInt(Seconds);
	}

	return SText;
}

