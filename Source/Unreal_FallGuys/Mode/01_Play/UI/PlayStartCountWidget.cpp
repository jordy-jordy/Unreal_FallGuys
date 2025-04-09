// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayStartCountWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "Global/FallGlobal.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"

// 델리게이트 테스트
//#include "Mode/01_Play/PlayGameMode.h"
#include "Mode/01_Play/PlayGameState.h"


void UPlayStartCountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 델리게이트 테스트 : 원본
	//APlayGameMode* PlayGameMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	//if (nullptr == PlayGameMode)
	//{
	//	return;
	//}
	//else
	//{
	//	PlayGameMode->RegisterWidgetDelegate(TEXT("StartCount"), 
	//		FWidgetDelegate::CreateUObject(this, &UPlayStartCountWidget::StartCountWidget)
	//	);
	//}
	// 델리게이트 테스트

	// 델리게이트 테스트 : 250409 이현정 : 동기화를 위해 GameState로 이동
	APlayGameState* FallState = GetWorld()->GetGameState<APlayGameState>();
	if (FallState)
	{
		FallState->RegisterWidgetDelegate(
			TEXT("StartCount"),
			FWidgetDelegate::CreateUObject(this, &UPlayStartCountWidget::StartCountWidget));
	}
	// 델리게이트 테스트
}

void UPlayStartCountWidget::SetWidgetImage(class UImage* _CountImage, TArray<class UTexture2D*> _ArrTexture, int _Index)
{
	_CountImage->SetBrushFromTexture(_ArrTexture[_Index]);
	_CountImage->SetColorAndOpacity(FLinearColor::White);
}

void UPlayStartCountWidget::StartCountWidget()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlayAnimation(CountAnim);
	CountWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("HiddenWidget"))));
	BindToAnimationFinished(CountAnim, CountWidgetAnimationEvent);
}

void UPlayStartCountWidget::HiddenWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}




