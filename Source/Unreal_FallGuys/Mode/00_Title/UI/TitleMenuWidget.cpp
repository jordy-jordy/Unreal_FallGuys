// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Components/CanvasPanelSlot.h"


void UTitleMenuWidget::SwitchWidgetInMenu(const FVector2D _Value)
{
	//UTitleUserWidget* CurWidget = GetCurUserWidget();
	//FString WidgetName = CurWidget->GetClass()->GetName();
	//EUIType CurType = CurWidget->GetCurUIType();

	//if (CurType == EUIType::CustomInven)
	//{
	//	CurType = EUIType::TitleCustom;
	//}

	//if (WidgetName.Contains(FString("TitleMenu")))
	//{
	//	CurType = CurWidget->GetCurUserWidget()->GetCurUIType();
	//}

	if (_Value.X > 0 && _Value.Y == 0 && GetCurUIType() == EUIType::TitleHome)
	{
		SwitchWidget(EUIType::TitleCustom);
		return;
	}
	else if (_Value.X < 0 && _Value.Y == 0 && GetCurUIType() == EUIType::TitleCustom)
	{
		SwitchWidget(EUIType::TitleHome);
		return;
	}
}
