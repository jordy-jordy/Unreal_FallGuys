// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitlePlayerCountWidget.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"


void UTitlePlayerCountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;
}

void UTitlePlayerCountWidget::WIdgetSelectInputAction(const FVector2D& _Value)
{
	Super::WIdgetSelectInputAction(_Value);

	if (GetMainWidget()->GetCurUIType() == ETitleUIType::TitlePlayerCount)
	{
		if ((_Value.X < 0.0f && _Value.Y == 0.0f) && true == InputCheckValue)
		{
			InputCheckValue = false;
			GetMainWidget()->SwitchWidget(ETitleUIType::TitleEntrance);
			return;
		}

		if (_Value.X > 0.0f && _Value.Y == 0.0f)
		{
			return;
		}
	}
}