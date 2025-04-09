// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayStartCountWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "Global/FallGlobal.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"

// ��������Ʈ �׽�Ʈ
//#include "Mode/01_Play/PlayGameMode.h"
#include "Mode/01_Play/PlayGameState.h"


void UPlayStartCountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ��������Ʈ �׽�Ʈ : ����
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
	// ��������Ʈ �׽�Ʈ

	// ��������Ʈ �׽�Ʈ : 250409 ������ : ����ȭ�� ���� GameState�� �̵�
	APlayGameState* FallState = GetWorld()->GetGameState<APlayGameState>();
	if (FallState)
	{
		FallState->RegisterWidgetDelegate(
			TEXT("StartCount"),
			FWidgetDelegate::CreateUObject(this, &UPlayStartCountWidget::StartCountWidget));
	}
	// ��������Ʈ �׽�Ʈ
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




