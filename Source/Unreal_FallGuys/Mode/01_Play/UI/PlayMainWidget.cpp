// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayMainWidget.h"
#include "Unreal_FallGuys/Unreal_FallGuys.h"
#include "Global/FallConst.h"
#include "Global/FallGlobal.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Mode/01_Play/PlayGameState.h"

// 임시
#include "Mode/01_Play/PlayPlayerState.h"
#include "Mode/01_Play/PlayCharacter.h"
#include "Mode/01_Play/UI/PlayResultWidget.h"

void UPlayMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ResultLevel이면 위젯 꺼지게
	if (true == UFallGlobal::GetIsResultLevel())
	{
		UPlayUserWidget* StandbyWIdget = FindWidget(EPlayUIType::PlayStandby);
		StandbyWIdget->SetVisibility(ESlateVisibility::Hidden);
	}

	// 이전 실패 플레이어 가져오기
	GetPrevFailPlayers();
}

void UPlayMainWidget::MainWidgetInit()
{
	UWidget* PanelWidget = GetWidgetFromName(UFallConst::UI::PlayMainPanelName);
	CanvasPanel = Cast<UCanvasPanel>(PanelWidget);

	if (nullptr == CanvasPanel)
	{
		return;
	}

	if (false == CanvasPanel->IsValidLowLevel())
	{
		return;
	}
}

void UPlayMainWidget::CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible)
{
	UPlayUserWidget* Widget = CreateWidget<UPlayUserWidget>(CanvasPanel, _Widget);

	if (nullptr == Widget)
	{
		return;
	}

	FString WidgetName = Widget->GetClass()->GetName();

	Widget->SetMainWidget(this);
	CanvasPanel->AddChild(Widget);

	SetWidgetUIType(WidgetName);

	Widgets.Emplace(UIType, Widget);

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	CanvasSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
	CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));

	if (false == _IsVisible)
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CurWidget = Widget;
		CurUIType = UIType;
	}
}

void UPlayMainWidget::SetWidgetUIType(FString _CurWidgetName)
{
	if (_CurWidgetName.Contains(FString("PlayStandby")))
	{
		UIType = EPlayUIType::PlayStandby;
	}
	else if (_CurWidgetName.Contains(FString("PlayInGame")))
	{
		UIType = EPlayUIType::PlayInGame;
	}
	else if (_CurWidgetName.Contains(FString("PlayLevelTag")))
	{
		UIType = EPlayUIType::PlayLevelTag;
	}
	else if (_CurWidgetName.Contains(FString("PlayStartCount")))
	{
		UIType = EPlayUIType::PlayStartCount;
	}
	else if (_CurWidgetName.Contains(FString("PlayReturnHome")))
	{
		UIType = EPlayUIType::PlayReturnHome;
	}
	else if (_CurWidgetName.Contains(FString("PlayResult")))
	{
		UIType = EPlayUIType::PlayResult;
	}
	else if (_CurWidgetName.Contains(FString("PlaySpectatorResult")))
	{
		UIType = EPlayUIType::PlaySpectatorResult;
	}
	else if (_CurWidgetName.Contains(FString("PlayGameOver")))
	{
		UIType = EPlayUIType::PlayGameOver;
	}
	else if (_CurWidgetName.Contains(FString("PlayClearCount")))
	{
		UIType = EPlayUIType::PlayClearCount;
	}
	else if (_CurWidgetName.Contains(FString("PlayScore")))
	{
		UIType = EPlayUIType::PlayScore;
	}
	else if (_CurWidgetName.Contains(FString("PlayLevelResult")))
	{
		UIType = EPlayUIType::PlayLevelResult;
	}
}

void UPlayMainWidget::SwitchWidget(EPlayUIType _UIType)
{
	TMultiMap<EPlayUIType, UPlayUserWidget*> AllWidget = GetAllWidgets();
	if (true == AllWidget.IsEmpty())
	{
		return;
	}

	// ChangeWidget : 주가 될, SelfHitTestInvisible 상태가 될 위젯
	UPlayUserWidget* ChangeWidget = *AllWidget.Find(_UIType);
	if (nullptr == ChangeWidget)
	{
		return;
	}

	// CurUserWidget : 현재 주가 되는 SelfHitTestInvisible 상태인 위젯
	UPlayUserWidget* CurUserWidget = GetCurWidget();

	if (CurUserWidget == ChangeWidget)
	{
		return;
	}

	//CurUIType 위젯 => _UIType 위젯
	switch (CurUIType)
	{
	case EPlayUIType::PlayStandby:
	{
		switch (EPlayUIType(_UIType))
		{
		case EPlayUIType::PlayLevelTag:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			break;
		}
		default:
			break;
		}
		break;
	}
	case EPlayUIType::PlayLevelTag:
	{
		switch (EPlayUIType(_UIType))
		{
		case EPlayUIType::PlayInGame:
		{
			APlayGameState* GameState = Cast<APlayGameState>(GetWorld()->GetGameState());
			if (nullptr == GameState)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayLevelTag->PlayInGame >> GameState is null"), *FString(__FUNCSIG__));
				return;
			}

			EStageType StageType = GameState->GetLevelType_STATE();

			UPlayUserWidget* ClearCount = FindWidget(EPlayUIType::PlayClearCount);
			UPlayUserWidget* PlayScore = FindWidget(EPlayUIType::PlayScore);

			if (StageType == EStageType::SOLO)
			{
				ClearCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			if (StageType == EStageType::TEAM)
			{
				PlayScore->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			break;
		}
		default:
			break;
		}
		break;
	}
	case EPlayUIType::PlayInGame:
	{
		switch (EPlayUIType(_UIType))
		{
		case EPlayUIType::PlayResult:
		{
			APlayGameState* GameState = Cast<APlayGameState>(GetWorld()->GetGameState());
			if (nullptr == GameState)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayInGame->PlayResult >> GameState is null"), *FString(__FUNCSIG__));
				return;
			}

			APlayCharacter* PlayCharacter = Cast<APlayCharacter>(GetOwningPlayerPawn());
			if (nullptr == PlayCharacter)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayResult->PlayInGame >> PlayCharacter is null"), *FString(__FUNCSIG__));
				return;
			}

			APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayCharacter->GetPlayerState());
			if (nullptr == PlayPlayerState)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayResult->PlayInGame >> PlayPlayerState is null"), *FString(__FUNCSIG__));
				return;
			}

			EStagePhase CurStagePhase = GameState->GetCurStagePhase_STATE();
			EPlayerStatus CurPlayerStatus = PlayPlayerState->GetPlayerStateStatus();

			if (EPlayerStatus::FAIL == CurPlayerStatus)
			{
				if (EStagePhase::STAGE_1 == CurStagePhase)
				{
					//AllWidgetHidden();
					ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			else if (EPlayerStatus::SUCCESS == CurPlayerStatus)
			{
				//AllWidgetHidden();
				//UPlayResultWidget* ResultWidget = Cast<UPlayResultWidget>(FindWidget(EPlayUIType::PlayResult));
				//ResultWidget->ChangeResources();
				ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			break;
		}
		default:
			break;
		}
		break;
	}
	case EPlayUIType::PlayResult:
	{
		switch (EPlayUIType(_UIType))
		{
		case EPlayUIType::PlayInGame:
		{
			APlayGameState* GameState = Cast<APlayGameState>(GetWorld()->GetGameState());
			if (nullptr == GameState)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayResult->PlayInGame >> GameState is null"), *FString(__FUNCSIG__));
				return;
			}

			APlayCharacter* PlayCharacter = Cast<APlayCharacter>(GetOwningPlayerPawn());
			if (nullptr == PlayCharacter)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayResult->PlayInGame >> PlayCharacter is null"), *FString(__FUNCSIG__));
				return;
			}

			APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayCharacter->GetPlayerState());
			if (nullptr == PlayPlayerState)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayResult->PlayInGame >> PlayPlayerState is null"), *FString(__FUNCSIG__));
				return;
			}

			EStageType StageType = GameState->GetLevelType_STATE();

			UPlayUserWidget* ClearCount = FindWidget(EPlayUIType::PlayClearCount);
			UPlayUserWidget* PlayScore = FindWidget(EPlayUIType::PlayScore);
			UPlayUserWidget* SpectatorResult = FindWidget(EPlayUIType::PlaySpectatorResult);
			UPlayResultWidget* Result = Cast<UPlayResultWidget>(CurUserWidget);

			if (StageType == EStageType::SOLO)
			{
				CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
				ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				ClearCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				SpectatorResult->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Result->SetSpectatorView(true);
			}
			else if (StageType == EStageType::TEAM)
			{
				UFallGlobal::SetCanMoveLevel(true);
			}

			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}

	SetCurWidget(ChangeWidget);
	CurWidget;
	CurUIType = _UIType;
}

void UPlayMainWidget::TargetWidgetHidden(EPlayUIType _Type)
{
	for (TPair<EPlayUIType, UPlayUserWidget*> Pair : Widgets)
	{
		if (Pair.Key == _Type)
		{
			Pair.Value->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPlayMainWidget::AllWidgetHidden()
{
	for (TPair<EPlayUIType, UPlayUserWidget*> AllWidgets : Widgets)
	{
		AllWidgets.Value->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayMainWidget::AllWidgetSelfHitTestInvisible()
{
	for (TPair<EPlayUIType, UPlayUserWidget*> AllWidgets : Widgets)
	{
		AllWidgets.Value->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

UPlayUserWidget* UPlayMainWidget::FindWidget(EPlayUIType _Type, int _Index/* = 0*/)
{
	for (TPair<EPlayUIType, UPlayUserWidget*> Pair : Widgets)
	{
		if (Pair.Key == _Type)
		{
			return Pair.Value;
		}
	}

	return nullptr;
}

bool UPlayMainWidget::IsFailPlayer()
{
	APlayCharacter* PlayCharacter = Cast<APlayCharacter>(GetOwningPlayerPawn());
	if (nullptr == PlayCharacter)
	{
		return false;
	}

	APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayCharacter->GetPlayerState());
	if (nullptr == PlayPlayerState)
	{
		return false;
	}

	APlayGameState* PlayGameState = Cast<APlayGameState>(GetWorld()->GetGameState());
	if (nullptr == PlayGameState)
	{
		return false;
	}

	EPlayerStatus CurPlayerStatus = PlayPlayerState->GetPlayerStateStatus();
	if (EPlayerStatus::FAIL == PlayPlayerState->GetPlayerStateStatus())
	{
		return true;
	}

	return false;
}

bool UPlayMainWidget::IsSuccessPlayer()
{
	APlayCharacter* PlayCharacter = Cast<APlayCharacter>(GetOwningPlayerPawn());
	if (nullptr == PlayCharacter)
	{
		return false;
	}

	APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayCharacter->GetPlayerState());
	if (nullptr == PlayPlayerState)
	{
		return false;
	}

	APlayGameState* PlayGameState = Cast<APlayGameState>(GetWorld()->GetGameState());
	if (nullptr == PlayGameState)
	{
		return false;
	}

	EPlayerStatus CurPlayerStatus = PlayPlayerState->GetPlayerStateStatus();
	if (EPlayerStatus::SUCCESS == PlayPlayerState->GetPlayerStateStatus())
	{
		return true;
	}

	return false;
}

TArray<FPlayerInfoEntry>* UPlayMainWidget::GetPrevFailPlayers()
{
	APlayGameState* GameState = Cast<APlayGameState>(GetWorld()->GetGameState());
	if (nullptr == GameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : GameState is null"), *FString(__FUNCSIG__));
		return nullptr;
	}

	APlayCharacter* PlayCharacter = Cast<APlayCharacter>(GetOwningPlayerPawn());
	if (nullptr == PlayCharacter)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayCharacter is null"), *FString(__FUNCSIG__));
		return nullptr;
	}

	APlayPlayerState* PlayerState = PlayCharacter->GetPlayerState<APlayPlayerState>();
	if (nullptr == PlayerState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayerState is null"), *FString(__FUNCSIG__));
		return nullptr;
	}

	PrevFailPlayers = GameState->FailPlayerInfoArray;

	return &PrevFailPlayers;
}