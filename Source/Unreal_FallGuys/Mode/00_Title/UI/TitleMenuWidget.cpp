// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Components/CanvasPanelSlot.h"


//void UTitleMenuWidget::SwitchWidgetInMenu(const FVector2D _Value)
//{
//	UTitleUserWidget* CurWg = GetCurUserWidget();
//	EUIType CurType = CurWg->GetCurUIType();
//
//	if (GetCurUserWidget()->GetCurUIType() != EUIType::TitleEntrance)
//	{
//		if (_Value.X > 0 && _Value.Y == 0 && GetCurUserWidget()->GetCurUIType() == EUIType::TitleHome)
//		{
//			SwitchWidget(EUIType::TitleCustom);
//			return;
//		}
//		else if (_Value.X < 0 && _Value.Y == 0 && GetCurUserWidget()->GetCurUIType() == EUIType::TitleCustom)
//		{
//			SwitchWidget(EUIType::TitleHome);
//			return;
//		}
//	}
//}
