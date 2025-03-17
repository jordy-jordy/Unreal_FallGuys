// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/UIInputManager.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"


void UUIInputManager::UIInput(const FVector2D _Value)
{
	//bool Value = InputCheck(_Value);

	//if (true == Value)
	//{
	//	SwitchWidgetHomeEnt(_Value);
	//}
	//else
	//{
	//	SwitchWidgetMenu(_Value);
	//}

	int a = 0;
}

bool UUIInputManager::SwitchWidgetMenu(const FVector2D _Value)
{
#ifdef WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%S(%u)> %s"), __FUNCTION__, __LINE__, *_Value.ToString()));
#endif

	EUIType CurType = GetMainWidget()->GetCurUIType();

	if (_Value.X > 0 && _Value.Y == 0 && CurType == EUIType::TitleHome)
	{
		GetMainWidget()->SwitchWidget(EUIType::TitleCustom);
		return true;
	}
	else if (_Value.X < 0 && _Value.Y == 0 && CurType == EUIType::TitleCustom)
	{
		GetMainWidget()->SwitchWidget(EUIType::TitleHome);
		return false;
	}

	return false;
}

bool UUIInputManager::SwitchWidgetHomeEnt(const FVector2D _Value)
{
#ifdef WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%S(%u)> %s"), __FUNCTION__, __LINE__, *_Value.ToString()));
#endif

	EUIType CurType = GetMainWidget()->GetCurUIType();

	if (_Value.X == 0 && _Value.Y < 0 && CurType == EUIType::TitleEntrance)
	{
		GetMainWidget()->SwitchWidget(EUIType::TitleHome);
		return true;
	}
	else if (_Value.X == 0 && _Value.Y > 0 && CurType == EUIType::TitleHome)
	{
		GetMainWidget()->SwitchWidget(EUIType::TitleEntrance);
		return false;
	}

	return false;
}
