// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayReturnHomeWidget.h"


void UPlayReturnHomeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;
}

void UPlayReturnHomeWidget::ToggleWidget()
{
	IsWidgetVisible = !IsWidgetVisible;

	if (true == IsWidgetVisible)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}