// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayMainWidget.h"
#include "Global/FallConst.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetSystemLibrary.h"


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
	
	UPlayUserWidget* ClearCount = FindWidget(EPlayUIType::PlayClearCount);
	UPlayUserWidget* StartCount = FindWidget(EPlayUIType::PlayStartCount);

	//CurUIType 위젯 => _UIType 위젯
	switch (CurUIType)
	{
	//case EPlayUIType::PlayStandby:
	//	switch (EPlayUIType(_UIType))
	//	{
	//	case EPlayUIType::PlayLevelTag:
	//		CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
	//		ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//		break;
	//	default:
	//		break;
	//	}
	//	break;
	//case EPlayUIType::PlayInGame:
	//	switch (EPlayUIType(_UIType))
	//	{
	//	case EPlayUIType::PlayStartCount:
	//		break;
	//	case EPlayUIType::PlayReturnHome:
	//		break;
	//	case EPlayUIType::PlayResult:
	//		break;
	//	case EPlayUIType::PlaySpectatorResult:
	//		break;
	//	case EPlayUIType::PlayGameOver:
	//		break;
	//	case EPlayUIType::PlayClearCount:
	//		break;
	//	case EPlayUIType::PlayScore:
	//		break;
	//	default:
	//		break;
	//	}
	//	break;
	
	case EPlayUIType::PlayLevelTag:
		switch (EPlayUIType(_UIType))
		{
		case EPlayUIType::PlayInGame:
			ClearCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			// 임시(나중에 주석 지우기)
			if (true == UFallConst::UseCountDown)
			{
				StartCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			break;
		default:
			break;
		}
		break;
	case EPlayUIType::PlayStartCount:
		break;
	case EPlayUIType::PlayReturnHome:
		break;
	case EPlayUIType::PlayResult:
		break;
	case EPlayUIType::PlaySpectatorResult:
		break;
	case EPlayUIType::PlayGameOver:
		break;
	case EPlayUIType::PlayClearCount:
		break;
	case EPlayUIType::PlayScore:
		break;
	case EPlayUIType::MAX:
		break;
	default:
		break;
	}

	CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
	ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetCurWidget(ChangeWidget);
	CurWidget;
	CurUIType = _UIType;
}

void UPlayMainWidget::WidgetHidden(EPlayUIType _Type)
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