// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayStartCountWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "Global/FallGlobal.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"


void UPlayStartCountWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayStartCountWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayStartCountWidget::SetWidgetImage(class UImage* _CountImage, TArray<class UTexture2D*> _ArrTexture, int _Index)
{
	_CountImage->SetBrushFromTexture(_ArrTexture[_Index]);
	_CountImage->SetColorAndOpacity(FLinearColor::White);
}



