// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleWidget.h"

#include <Unreal_FallGuys.h>
#include <Global/BaseGameInstance.h>


void UTitleWidget::SavedCostume(UGameInstance* _Ins, const FString& _CostumeName)
{
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Ins);
	GameIns->SaveSelectedCostume(_CostumeName);
}