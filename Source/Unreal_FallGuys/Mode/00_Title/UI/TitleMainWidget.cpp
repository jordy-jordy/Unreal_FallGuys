// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMainWidget.h"


UTitleMainWidget::UTitleMainWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UTitleMainWidget::SwitchWidgetInMain(const FVector2D _Value)
{
	UTitleUserWidget* CurWg = GetCurUserWidget();
	EUIType CurType = CurWg->GetCurUIType();

	if (GetCurUserWidget()->GetCurUIType() != EUIType::TitleCustom)
	{
		if (_Value.Y > 0 && _Value.X == 0 && GetCurUserWidget()->GetCurUIType() == EUIType::TitleHome)
		{
			SwitchWidget(EUIType::TitleEntrance);
			return;
		}
		else if (_Value.Y < 0 && _Value.X == 0 && GetCurUserWidget()->GetCurUIType() == EUIType::TitleEntrance)
		{
			SwitchWidget(EUIType::TitleHome);
			return;
		}
	}
}

