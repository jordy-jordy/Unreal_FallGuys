// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayStandbyWidget.h"
#include "Global/FallGlobal.h"
#include "Global/Data/GlobalDataTable.h"
#include "Global/BaseGameInstance.h"


FString UPlayStandbyWidget::GetCurLevelGuide()
{
	UBaseGameInstance* Inst = GetWorld()->GetGameInstance<UBaseGameInstance>();

	//if ("" == GuideText)
	//{
	//	return "";
	//}

	//const FPlayLevelDataRow* PlayLevelData = UGlobalDataTable::GetPlayLevelData(GetWorld(), GuideText);

	FString PlayGuide = Inst->InsGetRandomLevel();
	//FString CurGuideText = UFallGlobal::GetPlayGuideFromAssetName(PlayGuide);

	return PlayGuide;
}