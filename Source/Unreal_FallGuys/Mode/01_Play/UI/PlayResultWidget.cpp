// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayResultWidget.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"


void UPlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if ((true == GetMainWidget()->IsFailPlayer()) || (true == GetMainWidget()->IsSuccessPlayer()))
	{
		GetMainWidget()->AllWidgetHidden();
	}

	PlayAnimation(ResultAnim);
	ResultAnimEvent.BindUFunction(this, TEXT("ResultWidget"));
	BindToAnimationFinished(ResultAnim, ResultAnimEvent);
}

void UPlayResultWidget::ResultWidget()
{
	GetMainWidget()->SwitchWidget(EPlayUIType::PlayInGame);
}





