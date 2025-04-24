// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Unreal_FallGuys/Unreal_FallGuys.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"


void UTitleMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == MenuButtons.IsEmpty())
	{
		CurButton = MenuButtons[0];
	}
}

void UTitleMenuWidget::ChangeButtonImage(UObject* _BaseResource, UObject* _SelectResource)
{
	if (nullptr == _BaseResource)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : BaseResource is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == _SelectResource)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : SelectResource is null"), *FString(__FUNCSIG__));
		return;
	}

	UObject* ResourceObject = nullptr;

	if (true == MenuButtons.IsEmpty())
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : MenuButtons is Empty"), *FString(__FUNCSIG__));
		return;
	}

	for (int i = 0; i < MenuButtons.Num(); i++)
	{
		FButtonStyle ButtonStyle = MenuButtons[i]->GetStyle();
		FSlateBrush NormalBrush = MenuButtons[i]->GetStyle().Normal;

		if (MenuButtons[i] == CurButton)
		{
			ResourceObject = _SelectResource;
		}
		else
		{
			ResourceObject = _BaseResource;
		}

		NormalBrush.SetResourceObject(ResourceObject);
		ButtonStyle.SetNormal(NormalBrush);
		MenuButtons[i]->SetStyle(ButtonStyle);
	}
}

