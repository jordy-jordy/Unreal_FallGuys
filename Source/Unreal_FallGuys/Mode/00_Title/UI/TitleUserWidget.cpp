// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "Global/FallConst.h"
#include "Components/CanvasPanelSlot.h"


UTitleUserWidget::UTitleUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutomaticallyRegisterInputOnConstruction = true;
}

void UTitleUserWidget::InputCheck(const FVector2D& _Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%S(%u)> %s"), __FUNCTION__, __LINE__, *_Value.ToString()));
}

void UTitleUserWidget::WidgetInit(FName _PanelName)
{
	UWidget* PanelWidget = GetWidgetFromName(_PanelName);
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

void UTitleUserWidget::CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible)
{
	UTitleUserWidget* Widget = CreateWidget<UTitleUserWidget>(CanvasPanel, _Widget);

	if (nullptr == Widget)
	{
		return;
	}

	FString WidgetName = Widget->GetClass()->GetName();

	SetUserWidget(this);
	CanvasPanel->AddChild(Widget);

	if (WidgetName.Contains(FString("TitleHome")))
	{
		UIType = EUIType::TitleHome;
	}
	else if (WidgetName.Contains(FString("Custom")) || WidgetName.Contains(FString("Btn")))
	{
		UIType = EUIType::TitleCustom;
	}
	else if (WidgetName.Contains(FString("TitleEntrance")))
	{
		UIType = EUIType::TitleEntrance;
	}

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
		CurUserWidget = Widget;
	}
}

void UTitleUserWidget::AllWidgetHidden()
{
	for (TPair<EUIType, UTitleUserWidget*> AllWidgets : Widgets)
	{
		AllWidgets.Value->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTitleUserWidget::SwitchMenu(EUIType _UIType)
{
	TMultiMap<EUIType, UTitleUserWidget*> AllWidget = GetAllWidgets();

	UTitleUserWidget* Value = *AllWidget.Find(_UIType);
	if (nullptr == Value)
	{
		return;
	}

	UTitleUserWidget* CurWidget = GetCurUserWidget();
	if (CurWidget == Value)
	{
		return;
	}

	CurWidget->SetVisibility(ESlateVisibility::Hidden);
	Value->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetCurUserWidget(Value);
}