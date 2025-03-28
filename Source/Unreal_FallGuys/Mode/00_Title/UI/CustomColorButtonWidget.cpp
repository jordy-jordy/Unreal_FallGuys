// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomColorButtonWidget.h"
#include "Components/Button.h" // or the appropriate header where FButtonStyle is defined
#include <Global/GlobalEnum.h>
#include "Global/BaseGameInstance.h"
#include <Global/Data/GlobalDataTable.h>
#include "Materials/MaterialInterface.h"
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
	case ECostumeColor::DEFAULT:
		ColorName = "DEFAULT";
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
	case ECostumeColor::RED:
		ColorName = "RED";
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
	CustomName = GetCustomValueAsString(Color);
	UMaterialInterface* CustomMaterial= UFallGlobal::GetCostumeColorUIMaterial(GetOwningPlayerPawn(), CustomName);

	SetButtonStyle(CustomMaterial);

}

void UCustomColorButtonWidget::GetResourceFromName()
{
}


