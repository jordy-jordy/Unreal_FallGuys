// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Components/CanvasPanelSlot.h"


void UTitleMenuWidget::SwitchMenu(TSubclassOf<UUserWidget> _PrevWidget, TSubclassOf<UUserWidget> _NextWidget)
{
	UTitleUserWidget* PrevWidget = Cast<UTitleUserWidget>(_PrevWidget);

	if (ESlateVisibility::SelfHitTestInvisible == PrevWidget->GetVisibility())
	{
		PrevWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	TArray<UTitleUserWidget*> AllWidgets = GetAllWidgets();


}
