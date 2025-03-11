// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleEntranceWidget.h"
#include "Global/GlobalEnum.h"


void UTitleEntranceWidget::OpenValueInputPanel(bool _IsVisible)
{
	UTitleUserWidget* Widget = *GetAllWidgets().Find(EUIType::TitleIPPort);

	if (true == _IsVisible)
	{
		Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
}