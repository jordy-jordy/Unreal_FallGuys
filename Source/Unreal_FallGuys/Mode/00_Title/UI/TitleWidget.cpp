// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleWidget.h"

#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/BaseGameInstance.h>


void UTitleWidget::ChangeCurCostume(const FString& _CostumeName)
{
	APawn* Pawn = GetOwningPlayerPawn();
	UFallGlobal::ChangeCostume(Pawn, _CostumeName);
}
