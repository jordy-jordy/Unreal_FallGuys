// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Components/CanvasPanelSlot.h"


void UTitleMenuWidget::SwitchWidgetInMenu(const FVector2D _Value)
{
	if (_Value.X > 0 && _Value.Y == 0 && GetCurUIType() != EUIType::TitleCustom)
	{
		SwitchWidget(EUIType::TitleCustom);
	}
	else if (_Value.X < 0 && _Value.Y == 0 && GetCurUIType() != EUIType::TitleHome)
	{
		SwitchWidget(EUIType::TitleHome);
	}
}
