// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleEntranceWidget.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"
#include "Unreal_FallGuys/Unreal_FallGuys.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"


void UTitleEntranceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = false;

	if (false == ArrButtons.IsEmpty())
	{
		CurButton = ArrButtons[0];
	}

	if (nullptr != StartServerButton)
	{
		EntAnimStarted.BindUFunction(this, FName(TEXT("ChangeButtonImage_Select")));
		//EntAnimFinished.BindDynamic(this, &UTitleEntranceWidget::ChangeButtonImage_Base);
	}
	if (nullptr != CompeteButton)
	{
		EntAnimStarted.BindUFunction(this, FName(TEXT("ChangeButtonImage_Select")));
		//EntAnimFinished.BindDynamic(this, &UTitleEntranceWidget::ChangeButtonImage_Base);
	}
	if (nullptr != ConnectButton)
	{
		EntAnimStarted.BindUFunction(this, FName(TEXT("ChangeButtonImage_Select")));
		//EntAnimFinished.BindDynamic(this, &UTitleEntranceWidget::ChangeButtonImage_Base);
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
		BindToAnimationStarted(StartServerButton, EntAnimStarted);
		//BindToAnimationFinished(StartServerButton, EntAnimFinished);
		PlayAnimation(StartServerButton);
		break;
	}
	case 1:
	{
		BindToAnimationStarted(CompeteButton, EntAnimStarted);
		//BindToAnimationFinished(CompeteButton, EntAnimFinished);
		PlayAnimation(CompeteButton);
		break;
	}
	case 2:
	{
		BindToAnimationStarted(ConnectButton, EntAnimStarted);
		//BindToAnimationFinished(ConnectButton, EntAnimFinished);
		PlayAnimation(ConnectButton);
		break;
	}
	default:
		break;
	}

	IsInput = false;
}

void UTitleEntranceWidget::ChangeButtonImage(FString _ButtonState)
{
	if (nullptr == CurButton)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : CurButton is null"), *FString(__FUNCSIG__));
		return;
	}
	if (true == ArrBase.IsEmpty())
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : ArrBase is Empty"), *FString(__FUNCSIG__));
		return;
	}
	if (true == ArrSelect.IsEmpty())
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : ArrSelect is Empty"), *FString(__FUNCSIG__));
		return;
	}

	UObject* ResourceObject = nullptr;
	if (true == _ButtonState.Equals(TEXT("Base"), ESearchCase::IgnoreCase))
	{
		ResourceObject = ArrBase[CurIndex];
	}
	else if (true == _ButtonState.Equals(TEXT("Select"), ESearchCase::IgnoreCase))
	{
		ResourceObject = ArrSelect[CurIndex];
	}

	if (nullptr == ResourceObject)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : ResourceObject is null"), *FString(__FUNCSIG__));
		return;
	}

	FButtonStyle ButtonStyle = CurButton->WidgetStyle;
	FSlateBrush NormalBrush = CurButton->WidgetStyle.Normal;
	NormalBrush.SetResourceObject(ResourceObject);
	ButtonStyle.SetNormal(NormalBrush);
	CurButton->SetStyle(ButtonStyle);
}

void UTitleEntranceWidget::ChangeButtonImage_Base()
{
	ChangeButtonImage(TEXT("Base"));
}

void UTitleEntranceWidget::ChangeButtonImage_Select()
{
	ChangeButtonImage(TEXT("Select"));
}