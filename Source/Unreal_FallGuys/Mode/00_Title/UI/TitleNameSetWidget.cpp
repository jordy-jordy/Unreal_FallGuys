// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleNameSetWidget.h"


void UTitleNameSetWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UTitleNameSetWidget::IsPlayerSetName(FString _Name)
{
	if (_Name == TEXT(""))
	{
		return false;
	}

	return true;
}
