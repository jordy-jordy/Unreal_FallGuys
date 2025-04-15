// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMainWidget.h"
#include "Global/FallConst.h"
#include "Global/FallGlobal.h"
#include "Components/CanvasPanelSlot.h"


void UTitleMainWidget::NativeConstruct()
{
	// 게임 나가기 후 플레이어 상태 리셋 및 TitleNameWidget 안해도 되게
	bool HasName = UFallGlobal::GetHasNickname();

	if (true == HasName)
	{
		SwitchWidget(ETitleUIType::TitleHome);
		UFallGlobal::ResetPlayerCondition();
	}
}

void UTitleMainWidget::MainWidgetInit()
{
	UWidget* PanelWidget = GetWidgetFromName(UFallConst::UI::TitleMainPanelName);
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

void UTitleMainWidget::CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible)
{
	UTitleUserWidget* Widget = CreateWidget<UTitleUserWidget>(CanvasPanel, _Widget);

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

void UTitleMainWidget::SetWidgetUIType(FString _CurWidgetName)
{
	if (_CurWidgetName.Contains(FString("TitleHome")))
	{
		UIType = ETitleUIType::TitleHome;
	}
	else if (_CurWidgetName.Contains(FString("Customize")))
	{
		UIType = ETitleUIType::TitleCustom;
	}
	else if (_CurWidgetName.Contains(FString("TitleEntrance")))
	{
		UIType = ETitleUIType::TitleEntrance;
	}
	else if (_CurWidgetName.Contains(FString("CustomeInven")))
	{
		UIType = ETitleUIType::CustomeInven;
	}
	else if (_CurWidgetName.Contains(FString("Btn")))
	{
		UIType = ETitleUIType::InvenBtn;
	}
	else if (_CurWidgetName.Contains(FString("IPPort")))
	{
		UIType = ETitleUIType::TitleIPPort;
	}
	else if (_CurWidgetName.Contains(FString("TitleName")))
	{
		UIType = ETitleUIType::TitleName;
	}
	else if (_CurWidgetName.Contains(FString("TitleMenu")))
	{
		UIType = ETitleUIType::TitleMenu;
	}
	else if (_CurWidgetName.Contains(FString("TitlePlayerCount")))
	{
		UIType = ETitleUIType::TitlePlayerCount;
	}
}

void UTitleMainWidget::SwitchWidget(ETitleUIType _UIType)
{
	TMultiMap<ETitleUIType, UTitleUserWidget*> AllWidget = GetAllWidgets();
	if (true == AllWidget.IsEmpty())
	{
		return;
	}

	// ChangeWidget : 주가 될, SelfHitTestInvisible 상태가 될 위젯
	UTitleUserWidget* ChangeWidget = *AllWidget.Find(_UIType);
	if (nullptr == ChangeWidget)
	{
		return;
	}

	// CurUserWidget : 현재 주가 되는 SelfHitTestInvisible 상태인 위젯
	UTitleUserWidget* CurUserWidget = GetCurWidget();

	if (CurUserWidget == ChangeWidget)
	{
		return;
	}
	
	UTitleUserWidget* MenuWidget = FindWidget(ETitleUIType::TitleMenu);

	// CurUIType 위젯 => _UIType 위젯
	switch (CurUIType)
	{
	case ETitleUIType::TitleHome:
	{
		switch (ETitleUIType(_UIType))
		{
		case ETitleUIType::TitleCustom:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			for (TPair<ETitleUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == ETitleUIType::InvenBtn)
				{
					Pair.Value->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			break;
		}
		case ETitleUIType::TitleEntrance:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
		default:
			break;
		}
		break;
	}
	case ETitleUIType::TitleCustom:
	{
		switch (ETitleUIType(_UIType))
		{
		case ETitleUIType::TitleHome:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			for (TPair<ETitleUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == ETitleUIType::InvenBtn)
				{
					Pair.Value->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			break;
		}
		case ETitleUIType::CustomeInven:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			for (TPair<ETitleUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == ETitleUIType::InvenBtn)
				{
					Pair.Value->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			MenuWidget->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
		default:
			break;
		}
		break;
	}
	case ETitleUIType::TitleEntrance:
	{
		switch (ETitleUIType(_UIType))
		{
		case ETitleUIType::TitleHome:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		}
		case ETitleUIType::TitleIPPort:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
		case ETitleUIType::TitlePlayerCount:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
		default:
			break;
		}
		break;
	}
	case ETitleUIType::CustomeInven:
	{
		switch (ETitleUIType(_UIType))
		{
		case ETitleUIType::TitleCustom:
		{
			for (TPair<ETitleUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == ETitleUIType::InvenBtn)
				{
					Pair.Value->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		}
		default:
			break;
		}
		break;
	}
	case ETitleUIType::TitleIPPort:
	{
		switch (ETitleUIType(_UIType))
		{
		case ETitleUIType::TitleEntrance:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
		default:
			break;
		}
		break;
	}
	case ETitleUIType::TitlePlayerCount:
	{
		switch (ETitleUIType(_UIType))
		{
		case ETitleUIType::TitleEntrance:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
		default:
			break;
		}
		break;
	}
	case ETitleUIType::TitleName:
	{
		switch (ETitleUIType(_UIType))
		{
		case ETitleUIType::TitleHome:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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

void UTitleMainWidget::WidgetHidden(ETitleUIType _Type)
{
	for (TPair<ETitleUIType, UTitleUserWidget*> Pair : Widgets)
	{
		if (Pair.Key == _Type)
		{
			Pair.Value->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UTitleMainWidget::AllWidgetHidden()
{
	for (TPair<ETitleUIType, UTitleUserWidget*> AllWidgets : Widgets)
	{
		AllWidgets.Value->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTitleMainWidget::AllWidgetSelfHitTestInvisible()
{
	for (TPair<ETitleUIType, UTitleUserWidget*> AllWidgets : Widgets)
	{
		AllWidgets.Value->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

UTitleUserWidget* UTitleMainWidget::FindWidget(ETitleUIType _Type, int _Index/* = 0*/)
{
	if (ETitleUIType::InvenBtn == _Type)
	{
		TArray<UTitleUserWidget*> InvenBtn;
		Widgets.MultiFind(ETitleUIType::InvenBtn, InvenBtn);
		return InvenBtn[_Index];
	}

	for (TPair<ETitleUIType, UTitleUserWidget*> Pair : Widgets)
	{
		if (Pair.Key == _Type)
		{
			return Pair.Value;
		}
	}

	return nullptr;
}


//void UTitleMainWidget::SwitchWidgetInMain(const FVector2D _Value)
//{
//	UTitleUserWidget* CurWg = GetCurUserWidget();
//	EUIType CurType = CurWg->GetCurUIType();
//
//	if (GetCurUserWidget()->GetCurUIType() != EUIType::TitleCustom)
//	{
//		if (_Value.Y > 0 && _Value.X == 0 && GetCurUserWidget()->GetCurUIType() == EUIType::TitleHome)
//		{
//			SwitchWidget(EUIType::TitleEntrance);
//			return;
//		}
//		else if (_Value.Y < 0 && _Value.X == 0 && GetCurUserWidget()->GetCurUIType() == EUIType::TitleEntrance)
//		{
//			SwitchWidget(EUIType::TitleHome);
//			return;
//		}
//	}
//}

