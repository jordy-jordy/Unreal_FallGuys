// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleEntranceWidget.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"


void UTitleEntranceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = false;

	if (false == ArrButtons.IsEmpty())
	{
		CurButton = ArrButtons[0];
	}
}

void UTitleEntranceWidget::WIdgetSelectInputAction(const FVector2D& _Value)
{
	if (GetMainWidget()->GetCurUIType() == ETitleUIType::TitleEntrance)
	{
		if (_Value.X < 0 && _Value.Y == 0.0f)
		{
			GetMainWidget()->SwitchWidget(ETitleUIType::TitleHome);
		}
	}
}

void UTitleEntranceWidget::WIdgetMoveInputAction(const FVector2D& _Value)
{
	if (GetMainWidget()->GetCurUIType() == ETitleUIType::TitleEntrance)
	{
		if (false == ArrButtons.IsEmpty() && true == IsInput)
		{
			if (_Value.X < 0 && _Value.Y == 0.0f)
			{
				CurIndex -= 1;

				if (CurIndex < 0)
				{
					CurIndex = 0;
				}
			}

			if (_Value.X > 0 && _Value.Y == 0.0f)
			{
				CurIndex += 1;

				if (CurIndex > ArrButtons.Num() - 1)
				{
					CurIndex = ArrButtons.Num() - 1;
				}
			}

			CurButton = ArrButtons[CurIndex];
			PlayButtonAnim(CurIndex);
		}
	}
}

void UTitleEntranceWidget::PlayButtonAnim(int _CurIndex)
{
	switch (_CurIndex)
	{
	case 0:
	{
		PlayAnimation(StartServerButton);
		break;
	}
	case 1:
	{
		PlayAnimation(CompeteButton);
		break;
	}
	case 2:
	{
		PlayAnimation(ConnectButton);
		break;
	}
	default:
		break;
	}

	IsInput = false;
}

void UTitleEntranceWidget::ChangeButtonImage(UButton* _CurButton, class UObject* InResourceObject)
{
	FButtonStyle ButtonStyle = _CurButton->WidgetStyle;
	FSlateBrush NormalBrush = _CurButton->WidgetStyle.Normal;
	NormalBrush.SetResourceObject(InResourceObject);
	ButtonStyle.SetNormal(NormalBrush);
}