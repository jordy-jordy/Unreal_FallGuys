// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayStartCountWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "Unreal_FallGuys/Unreal_FallGuys.h"
#include "Global/FallGlobal.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"
#include "Mode/01_Play/UI/PlayResultWidget.h"

// 델리게이트 테스트
//#include "Mode/01_Play/PlayGameMode.h"
#include "Mode/01_Play/PlayGameState.h"


void UPlayStartCountWidget::NativeConstruct()
{
	Super::NativeConstruct();

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
	CountWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("AfterCountWidget"))));
	BindToAnimationFinished(CountAnim, CountWidgetAnimationEvent);
}

void UPlayStartCountWidget::AfterCountWidget()
{
	SetVisibility(ESlateVisibility::Hidden);

	APlayGameState* GameState = Cast<APlayGameState>(GetWorld()->GetGameState());
	if (nullptr == GameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : GameState is null"), *FString(__FUNCSIG__));
		return;
	}

	UPlayUserWidget* SpectatorResult = GetMainWidget()->FindWidget(EPlayUIType::PlaySpectatorResult);
	if (nullptr == SpectatorResult)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : SpectatorResult is null"), *FString(__FUNCSIG__));
		return;
	}

	if (true == GetMainWidget()->IsFailPlayer() && EStagePhase::STAGE_1 != GameState->GetCurStagePhase_STATE())
	{
		// 카운트 끝나면->관전자 위젯 나오게
		SpectatorResult->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UPlayResultWidget* Result = Cast<UPlayResultWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayResult));
		//Result->SetSpectatorView(true);
	}
}

