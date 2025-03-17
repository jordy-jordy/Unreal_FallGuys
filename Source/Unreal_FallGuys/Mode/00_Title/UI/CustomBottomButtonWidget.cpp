// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomBottomButtonWidget.h"

#include "Global/GlobalEnum.h"
#include <Global/Data/GlobalDataTable.h>

void UCustomBottomButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetButtonTopStyle();
}

void UCustomBottomButtonWidget::SetButtonTopStyle()
{
	const FCostumeDataRow* Data=UGlobalDataTable::GetCostumeData(GetWorld(),ImgName);


	auto s = Data->CostumeIMG;

	int a = 0;
	//SetButtonStyle(NewTexture, Data->CostumeIMG);

}


