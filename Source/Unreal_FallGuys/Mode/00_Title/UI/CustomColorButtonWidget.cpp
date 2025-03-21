// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomColorButtonWidget.h"
#include "Components/Button.h" // or the appropriate header where FButtonStyle is defined
#include <Global/GlobalEnum.h>
#include "Global/BaseGameInstance.h"
#include <Global/Data/GlobalDataTable.h>
#include <Global/FallGlobal.h>

//void UCustomColorButtonWidget::Tick(FGeometry MyGeometry, float InDeltaTime)
//{
//	Super::Tick(MyGeometry, InDeltaTime);
//
//	//int a = 0;
//}

void UCustomColorButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	

	SetButtonColor();
}

FString UCustomColorButtonWidget::GetCustomValueAsString(ECostumeColor _Color)
{
	FString ColorName = "";
	switch (_Color)
	{
	case ECostumeColor::NONE:
		break;
	case ECostumeColor::PINK:
		ColorName = "PINK";
		break;
	case ECostumeColor::YELLOW:
		ColorName = "YELLOW";

		break;
	case ECostumeColor::BLUE:
		ColorName = "BLUE";
		break;
	default:
		break;

	}

	return ColorName;
	//FString Data = UEnum::GetValueAsString(TEXT("/Script/Unreal_FallGuys.GlobalEnum.ECostumeColor"), _Color);

	
	//return Data;

	
}

void UCustomColorButtonWidget::ChangePawnColor()
{
	CustomName = GetCustomValueAsString(Color);

	UFallGlobal::ChangeCostumeColor(GetOwningPlayerPawn(), CustomName);
}

void UCustomColorButtonWidget::SetButtonColor()
{
	//LoadTexture

	// String Ȥ�� Enum Color �� �ָ� -> �ش� �̹��� ���ҽ� ��� �ִ� �Լ�
	switch (Color)
	{
	case ECostumeColor::NONE:
		break;
	case ECostumeColor::PINK:
		//SetButtonStyle(Mat,"/Game/BP/HUD/Custom/Mat/M_PinkColor.M_PinkColor");
		break;
	case ECostumeColor::YELLOW:

		//SetButtonStyle(Mat,"/Game/BP/HUD/Custom/Mat/M_YellowColor.M_YellowColor");
		break;
	case ECostumeColor::BLUE:
		//SetButtonStyle(Mat,"/Game/BP/HUD/Custom/Mat/M_BlueColor.M_BlueColor");
		break;
	default:
		break;

	}
}

void UCustomColorButtonWidget::GetResourceFromName()
{
}


