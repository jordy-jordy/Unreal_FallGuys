// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomTopButtonWidget.h"
#include "Mode/00_Title/TitlePawn.h"
#include "Global/GlobalEnum.h"
#include <Global/Data/GlobalDataTable.h>


void UCustomTopButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetButtonTopStyle();
}

void UCustomTopButtonWidget::SetButtonTopStyle()
{
	if (ImgName == "") return;
	const FCostumeDataRow* Data = UGlobalDataTable::GetCostumeData(GetWorld(), ImgName);
	UTexture2D* Texture = Data->CostumeIMG;

	SetButtonStyle(Texture);



}


