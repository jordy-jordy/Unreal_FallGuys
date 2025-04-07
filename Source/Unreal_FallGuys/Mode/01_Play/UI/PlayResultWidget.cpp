// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayResultWidget.h"


void UPlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(LeftBigAnim);
	PlayAnimation(LeftSmallAnim);
	PlayAnimation(RightBigAnim);
	PlayAnimation(RightSmallAnim);
	PlayAnimation(TextAnim);
	//BindToAnimationFinished(TextAnim, FWidgetAnimationDynamicEvent::BindUFunction(this, &UPlayResultWidget::HiddenWidget));
}

void UPlayResultWidget::HiddenWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}




