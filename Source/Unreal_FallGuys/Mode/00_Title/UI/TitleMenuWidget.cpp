// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Global/FallConst.h"

void UTitleMenuWidget::MenuWidgetInit()
{
	UWidget* PanelWidget = GetWidgetFromName(UFallConst::MenuPanelName);
	MenuCanvasPanel = Cast<UCanvasPanel>(PanelWidget);

	if (nullptr == MenuCanvasPanel)
	{
		return;
	}

	if (false == MenuCanvasPanel->IsValidLowLevel())
	{
		return;
	}
}

void UTitleMenuWidget::CreateChildWidget(TSubclassOf<UUserWidget> _Widget)
{
	UTitleUserWidget* Widget = CreateWidget<UTitleUserWidget>(MenuCanvasPanel, _Widget);

	if (nullptr == Widget)
	{
		return;
	}

	Widget->SetMainWidget(this);

	MenuCanvasPanel->AddChild(Widget);

	Wigets.Add(Widget);
}