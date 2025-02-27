// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleWidget.h"


void UTitleWidget::StartPlay(UWidget* _Panel, bool _IsVisible)
{
	if (true == _IsVisible)
	{
		_Panel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_Panel->SetVisibility(ESlateVisibility::Hidden);
	}
}