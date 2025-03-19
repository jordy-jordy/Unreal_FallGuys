// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitleHUD.h"
#include "Blueprint/UserWidget.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"
#include "Unreal_FallGuys.h"


void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr == MainWidgetClass)
	{
		UE_LOG(FALL_DEV_LOG, Fatal, TEXT("%S(%u)> if (nullptr == MainWidgetClass)"), __FUNCTION__, __LINE__);
	}

	MainWidget = CreateWidget<UTitleMainWidget>(GetWorld(), MainWidgetClass, TEXT("MainWidget"));
	MainWidget->AddToViewport();
}