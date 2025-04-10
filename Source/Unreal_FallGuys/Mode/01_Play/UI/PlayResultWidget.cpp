// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayResultWidget.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"


void UPlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResultAnimEvent.BindUFunction(this, FName(FString(TEXT("ResultWidget"))));
	BindToAnimationFinished(ResultAnim, ResultAnimEvent);

	PlayAnimation(ResultAnim);
}

void UPlayResultWidget::ResultWidget()
{
	GetMainWidget()->SwitchWidget(EPlayUIType::PlayInGame);
}






