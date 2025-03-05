// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Components/CanvasPanelSlot.h"


void UTitleMenuWidget::SwitchMenu()
{
	ESlateVisibility CurVisibility = GetCurUserWidget()->GetVisibility();

	if (ESlateVisibility::SelfHitTestInvisible == CurVisibility)
	{
		GetCurUserWidget()->SetVisibility(ESlateVisibility::Hidden);
	}

	TArray<UTitleUserWidget*> AllWidgets = GetAllWidgets();
}
