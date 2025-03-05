// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Global/FallConst.h"
#include "Components/CanvasPanelSlot.h"

//void UTitleMenuWidget::MenuWidgetInit()
//{
//	UWidget* PanelWidget = GetWidgetFromName(UFallConst::MenuPanelName);
//	MenuCanvasPanel = Cast<UCanvasPanel>(PanelWidget);
//
//	if (nullptr == MenuCanvasPanel)
//	{
//		return;
//	}
//
//	if (false == MenuCanvasPanel->IsValidLowLevel())
//	{
//		return;
//	}
//}

//void UTitleMenuWidget::CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible)
//{
//	UTitleUserWidget* Widget = CreateWidget<UTitleUserWidget>(MenuCanvasPanel, _Widget);
//
//	if (nullptr == Widget)
//	{
//		return;
//	}
//
//	Widget->SetMenuWidget(this);
//	MenuCanvasPanel->AddChild(Widget);
//
//
//	Widgets.Add(Widget);
//
//	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
//	CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
//	CanvasSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
//	CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));
//
//	if (false == _IsVisible)
//	{
//		Widget->SetVisibility(ESlateVisibility::Hidden);
//	}
//	else
//	{
//		Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//		CurWidget = Widget;
//	}
//}

void UTitleMenuWidget::SwitchMenu(FString _PrevPanel, FString _NextPanel)
{
	UWidget* PrevWidget = GetWidgetFromName(FName(*_PrevPanel));
	UWidget* NextWidget = GetWidgetFromName(FName(*_NextPanel));

	if (nullptr == PrevWidget || nullptr == NextWidget)
	{
		return;
	}

	UCanvasPanel* PrevPanel = Cast<UCanvasPanel>(PrevWidget);
	UCanvasPanel* NextPanel = Cast<UCanvasPanel>(NextWidget);

	if (nullptr == PrevPanel || nullptr == NextPanel)
	{
		return;
	}

	PrevPanel->SetVisibility(ESlateVisibility::Hidden);
	NextPanel->SetVisibility(ESlateVisibility::Visible);
}
