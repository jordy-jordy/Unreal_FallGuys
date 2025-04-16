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

	if (nullptr != GameOverAnim)
	{
		GameOverAnimEvent.BindUFunction(this, FName(FString(TEXT("MoveToResultLevel"))));
		BindToAnimationFinished(GameOverAnim, GameOverAnimEvent);
		PlayAnimation(GameOverAnim);
	}
}

void UPlayGameOverWidget::WidgetVisible()
{
	GetMainWidget()->AllWidgetHidden();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UPlayGameOverWidget::MoveToResultLevel()
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

	TArray<FPlayerInfoEntry>& PrevFailPlayersInfo = GameState->PlayerInfoArray;
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("Count__PrevFailPlayers Ptr: %p"), &PrevFailPlayersInfo);

	FString CurPlayerID = PlayerState->PlayerInfo.UniqueID;

	for (FPlayerInfoEntry PrevFailPlayerInfo : PrevFailPlayersInfo)
	{
		FString PrevFailPlayerID = PrevFailPlayerInfo.PlayerInfo.UniqueID;

		if (CurPlayerID == PrevFailPlayerID)
		{
			UFallGlobal::SetCanMoveLevel(true);
		}
		else
		{
			if (false == IsShowResult && false == IsResultAnimated)
			{
				SetVisibility(ESlateVisibility::Hidden);
				PlayResult->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else if (true == IsShowResult)
			{
				UFallGlobal::SetCanMoveLevel(true);
			}
		}
	}

	//if (false == IsShowResult && false == IsResultAnimated)
	//{
	//	SetVisibility(ESlateVisibility::Hidden);
	//	PlayResult->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//}
	//else
	//{
	//	UFallGlobal::SetCanMoveLevel(true);
	//}
}
