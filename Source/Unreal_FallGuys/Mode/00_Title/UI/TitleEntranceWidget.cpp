// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleEntranceWidget.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"
#include "Unreal_FallGuys/Unreal_FallGuys.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"
#include "Mode/00_Title/UI/TitlePlayerCountWidget.h"


void UTitleEntranceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == ArrButtons.IsEmpty())
	{
		CurButton = ArrButtons[0];
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
}

void UTitleEntranceWidget::ChangeButtonImage()
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

	for (int i = 0; i < ArrButtons.Num(); i++)
	{
		FButtonStyle ButtonStyle = ArrButtons[i]->GetStyle();
		FSlateBrush NormalBrush = ArrButtons[i]->GetStyle().Normal;

		if (ArrButtons[i] == CurButton)
		{
			ResourceObject = ArrSelect[i];
		}
		else
		{
			ResourceObject = ArrBase[i];
		}

		NormalBrush.SetResourceObject(ResourceObject);
		ButtonStyle.SetNormal(NormalBrush);
		ArrButtons[i]->SetStyle(ButtonStyle);
	}
}
