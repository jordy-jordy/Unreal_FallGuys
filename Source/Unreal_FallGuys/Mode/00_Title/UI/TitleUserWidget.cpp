 // Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleUserWidget.h"

void UTitleUserWidget::InputCheck(const FVector2D& _Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%S(%u)> %s"), __FUNCTION__, __LINE__, *_Value.ToString()));
}