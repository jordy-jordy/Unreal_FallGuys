// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMainWidget.h"
#include "Global/FallConst.h"
#include "Components/CanvasPanelSlot.h"


UTitleMainWidget::UTitleMainWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

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

	SetMainWidget(this);
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

	UTitleUserWidget* ChangeWidget = *AllWidget.Find(_UIType);
	if (nullptr == ChangeWidget)
	{
		return;
	}

	UTitleUserWidget* CurUserWidget = GetCurWidget();

	if (CurUserWidget == ChangeWidget)
	{
		return;
	}

	switch (EUIType(_UIType))
	{
	case EUIType::TitleHome:
	{
		UTitleUserWidget* MenuWidget = *AllWidget.Find(EUIType::TitleMenu);

		switch (CurUIType)
		{
		case EUIType::TitleEntrance:
		case EUIType::TitleName:
		{
			MenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		}
		case EUIType::TitleCustom:
		{
			for (TPair<EUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == EUIType::CustomInven)
				{
					Pair.Value->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			break;
		}
		default:
			break;
		}
	}
		break;
	case EUIType::TitleCustom:
	{
		switch (CurUIType)
		{
		case EUIType::TitleHome:
		{
			for (TPair<EUIType, UTitleUserWidget*> Pair : Widgets)
			{
				if (Pair.Key == EUIType::CustomInven)
				{
					Pair.Value->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}

			break;
		}
		//case EUIType::CustomInven:
		//	break;
		default:
			break;
		}
		break;
	}
	//case EUIType::TitleEntrance:
	//	break;
	//case EUIType::CustomInven:
	//{
	//	switch (CurUIType)
	//	{
	//	case EUIType::TitleCustom:
	//	{
	//		break;
	//	}
	//	default:
	//		break;
	//	}
	//	break;
	//}
	//case EUIType::TitleIPPort:
	//	break;
	//case EUIType::TitleName:
	//	break;
	//case EUIType::TitleMenu:
	//	break;
	//case EUIType::MAX:
	//	break;
	default:
		break;
	}

	CurUserWidget->SetVisibility(ESlateVisibility::Hidden);
	ChangeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetCurWidget(ChangeWidget);
	CurWidget;
	CurUIType = _UIType;
}

//void UTitleMainWidget::SwitchWidgetType(EUIType _UIType)
//{
//	int a = 0;
//}

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

