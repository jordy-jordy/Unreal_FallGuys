// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMainWidget.h"


UTitleMainWidget::UTitleMainWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UTitleMainWidget::SwitchWidgetInMain(const FVector2D _Value)
{
	if (_Value.Y > 0 && _Value.X == 0 && GetCurUIType() != EUIType::TitleEntrance)
	{
		SwitchWidget(EUIType::TitleEntrance);
	}
	else if (_Value.Y < 0 && _Value.X == 0 && GetCurUIType() == EUIType::TitleEntrance)
	{
		SwitchWidget(EUIType::TitleHome);
	}
}

