// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMainWidget.h"
#include "Global/FallConst.h"
#include "Components/CanvasPanelSlot.h"


UTitleMainWidget::UTitleMainWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetMainWidget(this);
}

void UTitleMainWidget::MainWidgetInit()
{
	UWidget* PanelWidget = GetWidgetFromName(UFallConst::UI::MainPanelName);
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
		UIType = EUIType::TitleHome;
	}
	else if (_CurWidgetName.Contains(FString("Customize")))
	{
		UIType = EUIType::TitleCustom;
	}
	else if (_CurWidgetName.Contains(FString("TitleEntrance")))
	{
		UIType = EUIType::TitleEntrance;
	}
	else if (_CurWidgetName.Contains(FString("Btn")))
	{
		UIType = EUIType::CustomInven;
	}
	else if (_CurWidgetName.Contains(FString("IPPort")))
	{
		UIType = EUIType::TitleIPPort;
	}
	else if (_CurWidgetName.Contains(FString("TitleName")))
	{
		UIType = EUIType::TitleName;
	}
	else if (_CurWidgetName.Contains(FString("TitleMenu")))
	{
		UIType = EUIType::TitleMenu;
	}
}

void UTitleMainWidget::SwitchWidget(EUIType _UIType)
{
	TMultiMap<EUIType, UTitleUserWidget*> AllWidget = GetAllWidgets();
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
	
	UTitleUserWidget* MenuWidget = FindWidget(EUIType::TitleMenu);

	// CurUIType 위젯 => _UIType 위젯
	switch (CurUIType)
	{
	case EUIType::TitleHome:
	{
		switch (EUIType(_UIType))
		{
		case EUIType::TitleCustom:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			for (TPair<EUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == EUIType::CustomInven)
				{
					Pair.Value->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			break;
		}
		case EUIType::TitleEntrance:
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
	case EUIType::TitleCustom:
	{
		switch (EUIType(_UIType))
		{
		case EUIType::TitleHome:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			for (TPair<EUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == EUIType::CustomInven)
				{
					Pair.Value->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			break;
		}
		case EUIType::CustomInven:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			for (TPair<EUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == EUIType::CustomInven)
				{
					Pair.Value->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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
	case EUIType::TitleEntrance:
	{
		switch (EUIType(_UIType))
		{
		case EUIType::TitleHome:
		{
			CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		}
		case EUIType::TitleIPPort:
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
	case EUIType::CustomInven:
	{
		switch (EUIType(_UIType))
		{
		case EUIType::TitleCustom:
		{
			for (TPair<EUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == EUIType::CustomInven)
				{
					Pair.Value->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		}
		default:
			break;
		}
		break;
	}
	case EUIType::TitleIPPort:
	{
		switch (EUIType(_UIType))
		{
		case EUIType::TitleEntrance:
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
	case EUIType::TitleName:
	{
		switch (EUIType(_UIType))
		{
		case EUIType::TitleHome:
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

void UTitleMainWidget::WidgetHidden(EUIType _Type)
{
	for (TPair<EUIType, UTitleUserWidget*> Pair : Widgets)
	{
		if (Pair.Key == _Type)
		{
			Pair.Value->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UTitleMainWidget::AllWidgetHidden()
{
	for (TPair<EUIType, UTitleUserWidget*> AllWidgets : Widgets)
	{
		AllWidgets.Value->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTitleMainWidget::AllWidgetSelfHitTestInvisible()
{
	for (TPair<EUIType, UTitleUserWidget*> AllWidgets : Widgets)
	{
		AllWidgets.Value->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

UTitleUserWidget* UTitleMainWidget::FindWidget(EUIType _Type, int _Index/* = 0*/)
{
	if (EUIType::CustomInven != _Type)
	{
		for (TPair<EUIType, UTitleUserWidget*> Pair : Widgets)
		{
			if (Pair.Key == _Type)
			{
				return Pair.Value;
			}
		}
	}

	TArray<UTitleUserWidget*> CustomInvens;
	Widgets.MultiFind(EUIType::CustomInven, CustomInvens);
	return CustomInvens[_Index];
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

