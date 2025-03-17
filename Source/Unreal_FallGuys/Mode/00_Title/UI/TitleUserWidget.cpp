// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "Global/FallConst.h"
#include "Components/CanvasPanelSlot.h"


UTitleUserWidget::UTitleUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutomaticallyRegisterInputOnConstruction = true;
}

bool UTitleUserWidget::InputCheck(const FVector2D _Value)
{
	if (_Value.X == 0)
	{
		return true;
	}

	return false;
}
