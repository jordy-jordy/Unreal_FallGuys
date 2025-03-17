// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Mode/00_Title/UI/UIInputManager.h"


UTitleMenuWidget::UTitleMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UIInputManager = CreateDefaultSubobject<UUIInputManager>(TEXT("UIInputManager"));
}

void UTitleMenuWidget::UIInput(const FVector2D _Value)
{
	UIInputManager->UIInput(_Value);
}