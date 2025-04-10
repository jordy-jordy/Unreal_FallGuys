// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayScoreWidget.h"
#include "Mode/01_Play/TeamPlayGameState.h"
#include "TimerManager.h"


void UPlayScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//CurLimitTime = GetLimitTime();

	//TimerDelegate.BindUFunction(this, TEXT("LimitTimeDown"));
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, true);
}

int UPlayScoreWidget::GetTimeLimit_Minutes()
{
	//ATeamPlayGameState* TeamPlayGameState = GetWorld()->GetGameState<ATeamPlayGameState>();
	//float LimitTime = TeamPlayGameState->GetStageLimitTime();

	int Minutes = static_cast<int>(CurLimitTime / 60);

	return Minutes;
}

int UPlayScoreWidget::GetTimeLimit_Seconds()
{
	//ATeamPlayGameState* TeamPlayGameState = GetWorld()->GetGameState<ATeamPlayGameState>();
	//float LimitTime = TeamPlayGameState->GetStageLimitTime();

	int Seconds = static_cast<int>(CurLimitTime) % 60;

	return Seconds;
}

//float UPlayScoreWidget::GetLimitTime()
//{
//	ATeamPlayGameState* TeamPlayGameState = GetWorld()->GetGameState<ATeamPlayGameState>();
//	if (nullptr == TeamPlayGameState)
//	{
//		return 0.0f;
//	}
//
//	return TeamPlayGameState->GetStageLimitTime();
//}
//
//void UPlayScoreWidget::LimitTimeDown()
//{
//	if (CurLimitTime <= 0.0f)
//	{
//		//카운트다운이 끝났으므로 타이머 실행을 중지
//		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
//		return;
//	}
//
//	CurLimitTime -= 1.0f;
//}
