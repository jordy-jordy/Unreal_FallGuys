// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayStandbyWidget.h"
#include "Unreal_FallGuys/Unreal_FallGuys.h"
#include "Unreal_FallGuys.h"
#include "Global/Data/GlobalDataTable.h"
#include "Global/BaseGameInstance.h"
#include "Global/FallGlobal.h"


void UPlayStandbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FString UPlayStandbyWidget::GetCurLevelGuide()
{
	UBaseGameInstance* Inst = GetWorld()->GetGameInstance<UBaseGameInstance>();

	if (nullptr == Inst)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("nullptr == Inst"));
		return "";
	}

	FString PlayGuide = Inst->InsGetRandomLevel();

	return PlayGuide;
}

int UPlayStandbyWidget::GetMinPlayerNum()
{
	return UFallGlobal::GetMaxPlayerCount();
}