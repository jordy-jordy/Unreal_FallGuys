// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleWidget.h"

#include <Unreal_FallGuys.h>
#include <Global/BaseGameInstance.h>


void UTitleWidget::StartPlay(UWidget* _TitlePanel, UWidget* _StartPanel, bool _IsVisible, const FVector2D& _Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%S(%u)> %s"), __FUNCTION__, __LINE__, *_Value.ToString()));

	if (true == _IsVisible)
	{
		_TitlePanel->SetVisibility(ESlateVisibility::Visible);
		_StartPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_TitlePanel->SetVisibility(ESlateVisibility::Hidden);
		_StartPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTitleWidget::SavedCostume(UGameInstance* _Ins, const FString& _CostumeName)
{
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Ins);
	GameIns->SaveSelectedCostume(_CostumeName);
}
