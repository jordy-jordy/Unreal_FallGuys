// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayGameOverWidget.h"


void UPlayGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(GameOverAnim);
}

void UPlayGameOverWidget::AnimationDebug()
{
	PlayAnimation(GameOverAnim);
}
