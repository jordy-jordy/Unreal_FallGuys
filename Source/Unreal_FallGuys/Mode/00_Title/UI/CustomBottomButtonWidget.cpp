// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomBottomButtonWidget.h"

#include "Global/GlobalEnum.h"
#include <Global/Data/GlobalDataTable.h>

void UCustomBottomButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetButtonBottomStyle();
}

void UCustomBottomButtonWidget::SetButtonBottomStyle()
{
	if (ImgName == "") return;

	const FCostumeDataRow* Data=UGlobalDataTable::GetCostumeData(GetWorld(),ImgName);


	UTexture2D* Texture = Data->CostumeIMG;


	SetButtonStyle(Texture);

}


