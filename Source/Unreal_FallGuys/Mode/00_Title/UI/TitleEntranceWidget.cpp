// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleEntranceWidget.h"
#include "Global/GlobalEnum.h"


void UTitleEntranceWidget::VisibleInputPanel(UTitleUserWidget* _CurWidget)
{
	if (_CurWidget->GetName().Contains("IPPort"))
	{
		_CurWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UTitleUserWidget* Widget = *_CurWidget->GetAllWidgets().Find(EUIType::TitleIPPort);
		Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}