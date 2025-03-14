// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomBottomButtonWidget.h"
#include "Mode/00_Title/TitlePawn.h"
#include "Global/GlobalEnum.h"

void UCustomBottomButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetButtonTopStyle();
}

void UCustomBottomButtonWidget::SetButtonTopStyle()
{

	//SetButtonStyle(NewTexture, "/Script/Engine.Texture2D'/Game/Resources/UI/Custom/Character/UI_Icon_Bottom_Tanager_Variant01.UI_Icon_Bottom_Tanager_Variant01'");

}

void UCustomBottomButtonWidget::AttachCustomStaticMesh()
{

	//static mesh load

	ATitlePawn* MyPawn = Cast<ATitlePawn>(GetOwningPlayerPawn());


	MyPawn->AttachCustomStaticMesh(Type, ImgName);
}
