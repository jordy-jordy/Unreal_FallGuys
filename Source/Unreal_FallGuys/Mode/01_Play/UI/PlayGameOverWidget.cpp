// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayGameOverWidget.h"


void UPlayGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(BarPatternAnim);
	PlayAnimation(UpperStrokeAnim);
	PlayAnimation(LowerStroke1Anim);
	PlayAnimation(LowerStroke2Anim);
	PlayAnimation(RightBigAnim);
	PlayAnimation(LeftBigAnim);
	PlayAnimation(RIghtSmallAnim);
	PlayAnimation(LeftSmallAnim);
	PlayAnimation(TextAnim);
}

void UPlayGameOverWidget::AnimationDebug()
{
	PlayAnimation(BarPatternAnim);
	PlayAnimation(UpperStrokeAnim);
	PlayAnimation(LowerStroke1Anim);
	PlayAnimation(LowerStroke2Anim);
	PlayAnimation(RightBigAnim);
	PlayAnimation(LeftBigAnim);
	PlayAnimation(RIghtSmallAnim);
	PlayAnimation(LeftSmallAnim);
	PlayAnimation(TextAnim);
}
