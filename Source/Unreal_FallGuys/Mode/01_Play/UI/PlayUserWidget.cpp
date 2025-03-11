// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayUserWidget.h"
#include "Global/FallConst.h"
#include "Components/CanvasPanelSlot.h"


void UPlayUserWidget::WidgetInit(FName _PanelName)
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

void UPlayUserWidget::CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible)
{
	UPlayUserWidget* Widget = CreateWidget<UPlayUserWidget>(CanvasPanel, _Widget);

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
	else if (WidgetName.Contains(FString("Customize")))
	{
		UIType = EUIType::TitleCustom;
	}
	else if (WidgetName.Contains(FString("TitleEntrance")))
	{
		UIType = EUIType::TitleEntrance;
	}
	else if (WidgetName.Contains(FString("Btn")))
	{
		UIType = EUIType::CustomInven;
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