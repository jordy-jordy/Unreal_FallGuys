// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayGameOverWidget.h"
#include "Global/FallGlobal.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"
#include "Mode/01_Play/UI/PlayInGameWidget.h"
#include "Mode/01_Play/UI/PlayResultWidget.h"
#include "Mode/01_Play/PlayCharacter.h"
#include "Unreal_FallGuys/Unreal_FallGuys.h"

// 델리게이트 테스트
//#include "Mode/01_Play/PlayGameMode.h"
#include "Mode/01_Play/PlayGameState.h"


void UPlayGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 델리게이트 테스트 : 250409 이현정 : 동기화를 위해 GameState로 이동
	APlayGameState* FallState = GetWorld()->GetGameState<APlayGameState>();
	if (FallState)
	{
		FallState->RegisterWidgetDelegate(
			TEXT("GameOver"),
			FWidgetDelegate::CreateUObject(this, &UPlayGameOverWidget::WidgetVisible));
	}
	// 델리게이트 테스트
}

void UPlayGameOverWidget::WidgetVisible()
{
	GetMainWidget()->AllWidgetHidden();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UPlayInGameWidget* InGameWidget = Cast<UPlayInGameWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayInGame));
	UPlayResultWidget* PlayResult = Cast<UPlayResultWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayResult));
	bool IsShowResult = InGameWidget->GetShowResult();
	bool IsResultAnimated = PlayResult->GetAnimatedStatus();

	if (false == IsShowResult && false == IsResultAnimated)
	{
		if (nullptr != GameOverAnim)
		{
			GameOverAnimEvent.BindUFunction(this, FName(FString(TEXT("AfterGameOverAnim"))));
			UnbindAllFromAnimationFinished(GameOverAnim_Clear);
			BindToAnimationFinished(GameOverAnim, GameOverAnimEvent);
			PlayAnimation(GameOverAnim);
		}
	}
	else
	{
		if (nullptr != GameOverAnim_Clear)
		{
			GameOverAnim_ClearEvent.BindUFunction(this, FName(FString(TEXT("AfterGameOverAnim"))));
			UnbindAllFromAnimationFinished(GameOverAnim);
			BindToAnimationFinished(GameOverAnim_Clear, GameOverAnim_ClearEvent);
			PlayAnimation(GameOverAnim_Clear);
		}
	}
}

void UPlayGameOverWidget::AfterGameOverAnim()
{
	UPlayInGameWidget* InGameWidget = Cast<UPlayInGameWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayInGame));
	UPlayResultWidget* PlayResult = Cast<UPlayResultWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayResult));
	bool IsShowResult = InGameWidget->GetShowResult();
	bool IsResultAnimated = PlayResult->GetAnimatedStatus();

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

	// ---------------------------- 문제부분 ----------------------------------------------
	
	APlayCharacter* PlayCharacter = Cast<APlayCharacter>(GetOwningPlayerPawn());
	if (nullptr == PlayCharacter)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayCharacter is null"), *FString(__FUNCSIG__));
		return;
	}

	APlayPlayerState* PlayerState = PlayCharacter->GetPlayerState<APlayPlayerState>();
	if (nullptr == PlayerState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayerState is null"), *FString(__FUNCSIG__));
		return;
	}

	TArray<FPlayerInfoEntry> PrevFailPlayers = *GetMainWidget()->GetPrevFailPlayers();
	for (FPlayerInfoEntry PrevFailPlayer : PrevFailPlayers)
	{
		FString PrevFailPlayerID = PrevFailPlayer.PlayerInfo.UniqueID;
		FString CurPlayerID = PlayerState->PlayerInfo.UniqueID;

		if (CurPlayerID == PrevFailPlayerID)
		{
			int a = 0;
		}
	}

	// ---------------------------- 문제부분 ----------------------------------------------

	if (false == IsShowResult && false == IsResultAnimated)
	{
		SetVisibility(ESlateVisibility::Hidden);
		PlayResult->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		//UFallGlobal::SetCanMoveLevel(true);
	}
}
