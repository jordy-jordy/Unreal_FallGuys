// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomColorButtonWidget.h"
#include "Components/Button.h" // or the appropriate header where FButtonStyle is defined
#include <Global/GlobalEnum.h>
#include "Global/BaseGameInstance.h"
#include <Global/Data/GlobalDataTable.h>
#include "Materials/MaterialInterface.h"
#include <Global/FallGlobal.h>

void UCustomColorButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 색상 세팅
	SetButtonColor();
}

// 컬러 종류에 따라 컬러 네임 가져오기
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
	
}

// 타이틀폰 컬러 변환 실행 함수
void UCustomColorButtonWidget::ChangePawnColor()
{
	CustomName = GetCustomValueAsString(Color);

	UFallGlobal::ChangeCostumeColor(GetOwningPlayerPawn(), CustomName);
}


// 버튼 색상 세팅
void UCustomColorButtonWidget::SetButtonColor()
{
	CustomName = GetCustomValueAsString(Color);

	// 데이터 테이블에서 컬러 이미지 이름에 맞는 Material 가져옴
	UMaterialInterface* CustomMaterial= UFallGlobal::GetCostumeColorUIMaterial(GetOwningPlayerPawn(), CustomName); 

	SetButtonStyle(CustomMaterial);

}


