// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleCustomizingMainWidget.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"


void UTitleCustomizingMainWidget::WIdgetInputAction(const FVector2D& _Value)
{
	if (GetMainWidget()->GetCurUIType() == ETitleUIType::TitleCustom)
	{
		if (_Value.X < 0 && _Value.Y == 0.0f)
		{
			GetMainWidget()->SwitchWidget(ETitleUIType::TitleHome);
		}
	}
}