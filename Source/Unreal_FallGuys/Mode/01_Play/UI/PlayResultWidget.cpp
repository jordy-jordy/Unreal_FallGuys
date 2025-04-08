// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayResultWidget.h"
#include "Mode/01_Play/PlayGameState.h"


bool UPlayResultWidget::GetGameFinished()
{
	APlayGameState* PlayGameState = Cast<APlayGameState>(GetWorld()->GetGameState());

	if (nullptr == PlayGameState)
	{
		return false;
	}

	int GoalNum = PlayGameState->GetGameStateFinishPlayer();
	int TargetNum = PlayGameState->GetGameStateCurFinishPlayer();

	if (GoalNum == TargetNum)
	{
		return true;
	}

	return false;
}

//void UPlayResultWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	PlayAnimation(ResultAnim);
//	BindToAnimationFinished(ResultAnim, FWidgetAnimationDynamicEvent::BindUFunction(this, &UPlayResultWidget::HiddenWidget));
//}
//
//void UPlayResultWidget::HiddenWidget()
//{
//	SetVisibility(ESlateVisibility::Hidden);
//}




