// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomColorButtonWidget.h"

void UCustomColorButtonWidget::ButtonColor()
{
	switch (Color)
	{
	case EColor::TEST00:
		if (nullptr != Button)
		{
			//// 이것도 글로벌에서 가져오기
			//UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/BP/HUD/Custom/Mat/M_YellowColor.M_YellowColor"));

			//FButtonStyle ButtonStyle = Button->WidgetStyle;
			//ButtonStyle.SetNormal(FSlateBrush());
			//ButtonStyle.Normal.SetResourceObject(NewTexture);
			//Button->SetStyle(ButtonStyle);
		}
		break;
	case EColor::TEST01:
		break;
	case EColor::TEST02:
		break;
	default:
		break;
	}
}
