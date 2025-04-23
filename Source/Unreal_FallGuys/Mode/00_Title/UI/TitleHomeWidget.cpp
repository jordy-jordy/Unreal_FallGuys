// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleHomeWidget.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"


void UTitleHomeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = false;
}

void UTitleHomeWidget::WIdgetMenuInputAction(const FVector2D& _Value)
{
	Super::WIdgetMenuInputAction(_Value);

	if (GetMainWidget()->GetCurUIType() == ETitleUIType::TitleHome)
	{
		if (_Value.X > 0 && _Value.Y == 0.0f)
		{
			GetMainWidget()->SwitchWidget(ETitleUIType::TitleCustom);
			return;
		}
	}
}

void UTitleHomeWidget::WIdgetSelectInputAction(const FVector2D& _Value)
{
	Super::WIdgetSelectInputAction(_Value);
	
	if (GetMainWidget()->GetCurUIType() == ETitleUIType::TitleHome)
	{
		if (_Value.X > 0 && _Value.Y == 0.0f && true == InputCheckValue)
		{
			InputCheckValue = false;
			GetMainWidget()->SwitchWidget(ETitleUIType::TitleEntrance);
			return;
		}
	}
}