// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayScoreWidget.h"
#include "Mode/01_Play/TeamPlayGameState.h"
#include "TimerManager.h"
#include "Global/FallGlobal.h"


void UPlayScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FString UPlayScoreWidget::GetTimeLimit_Minutes(FString _Purpose)
{
	int CurLimitTime = 0.0f;
	if (_Purpose.Equals(TEXT("Whole"), ESearchCase::IgnoreCase))
	{
		CurLimitTime = static_cast<int>(UFallGlobal::GetStageLimitTime());
	}
	else if (_Purpose.Equals(TEXT("Remain"), ESearchCase::IgnoreCase))
	{
		CurLimitTime = static_cast<int>(UFallGlobal::GetRemainingTime());
	}

	int Minutes = static_cast<int>(CurLimitTime / 60);

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

FString UPlayScoreWidget::GetTimeLimit_Seconds(FString _Purpose)
{
	int CurLimitTime = 0.0f;
	if (_Purpose.Equals(TEXT("Whole"), ESearchCase::IgnoreCase))
	{
		CurLimitTime = static_cast<int>(UFallGlobal::GetStageLimitTime());
	}
	else if (_Purpose.Equals(TEXT("Remain"), ESearchCase::IgnoreCase))
	{
		CurLimitTime = static_cast<int>(UFallGlobal::GetRemainingTime());
	}

	int Seconds = static_cast<int>(CurLimitTime) % 60;

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

FString UPlayScoreWidget::GetLimitTrigger_Minutes(int _Value)
{
	int LimitTime = static_cast<int>(UFallGlobal::GetStageLimitTime()) - _Value;
	int Minutes = static_cast<int>(LimitTime / 60);

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

FString UPlayScoreWidget::GetLimitTrigger_Seconds(int _Value)
{
	int LimitTime = static_cast<int>(UFallGlobal::GetStageLimitTime()) - _Value;
	int Seconds = static_cast<int>(LimitTime) % 60;

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


