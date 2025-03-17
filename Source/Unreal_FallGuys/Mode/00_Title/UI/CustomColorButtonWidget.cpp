// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomColorButtonWidget.h"
#include "Components/Button.h" // or the appropriate header where FButtonStyle is defined

#include "Global/BaseGameInstance.h"
#include <Global/Data/GlobalDataTable.h>

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
	FString Data = UEnum::GetValueAsString(TEXT("/Script/Unreal_FallGuys.GlobalEnum.ECostumeColor"), _Color);
	return Data;
}

void UCustomColorButtonWidget::ChangePawnColor(ECostumeColor color)
{
	CustomName = GetCustomValueAsString(color);
	const FCostumeColorDataRow* CostumeColorData = UGlobalDataTable::GetCostumeColorData(GetWorld(), CustomName);

	
	if (CostumeColorData && CostumeColorData->CostumeMesh)
	{
		GetGameInstance<UBaseGameInstance>()->InsChangeCostume(GetOwningPlayerPawn(), CustomName);

	}
}

void UCustomColorButtonWidget::SetButtonColor()
{
	//LoadTexture

	// String 혹은 Enum Color 를 주면 -> 해당 이미지 리소스 경로 주는 함수
	switch (Color)
	{
	case ECostumeColor::NONE:
		break;
	case ECostumeColor::PINK:
		SetButtonStyle(Mat,"/Game/BP/HUD/Custom/Mat/M_PinkColor.M_PinkColor");
		break;
	case ECostumeColor::YELLOW:

		SetButtonStyle(Mat,"/Game/BP/HUD/Custom/Mat/M_YellowColor.M_YellowColor");
		break;
	case ECostumeColor::BLUE:
		SetButtonStyle(Mat,"/Game/BP/HUD/Custom/Mat/M_BlueColor.M_BlueColor");
		break;
	default:
		break;

	}
}

void UCustomColorButtonWidget::GetResourceFromName()
{
}


