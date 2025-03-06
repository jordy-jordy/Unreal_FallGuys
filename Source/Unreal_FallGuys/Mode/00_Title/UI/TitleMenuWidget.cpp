// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Components/CanvasPanelSlot.h"


void UTitleMenuWidget::SwitchMenu()
{
	TArray<UTitleUserWidget*> AllWidgets = GetAllWidgets();
	for (int i = 0; i < AllWidgets.Num(); i++)
	{
		if (ESlateVisibility::Hidden == AllWidgets[i]->GetVisibility())
		{
			AllWidgets[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SetCurUserWidget(AllWidgets[i]);
			UTitleUserWidget* CurWidget = GetCurUserWidget();
		}
		else if (ESlateVisibility::SelfHitTestInvisible == AllWidgets[i]->GetVisibility())
		{
			AllWidgets[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
