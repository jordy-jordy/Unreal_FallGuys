// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayLevelTagWidget.h"


void UPlayLevelTagWidget::StartPlayAnim()
{
	PlayAnimation(LevelNamePartPanelAnim);
	PlayAnimation(TagWhitePartAnim);
	PlayAnimation(LevelExpPartPanelAnim);
}
