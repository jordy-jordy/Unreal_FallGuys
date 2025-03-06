// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomColorButtonWidget.h"
#include "Components/Button.h" // or the appropriate header where FButtonStyle is defined

#include "Global/BaseGameInstance.h"

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

void UCustomColorButtonWidget::SetButtonStyle(FString _Path)
{
	FText Path = FText::FromString(_Path);

	UMaterial* Texture = LoadObject<UMaterial>(nullptr, *_Path);

	if (Texture)
	{
		//FButtonStyle ButtonStyle = Button->GetStyle();
		//ButtonStyle.Normal.SetResourceObject(Texture);
		//Button->SetStyle(ButtonStyle);

		Img_color->SetBrushResourceObject(Texture);
		
		FSlateBrush& Brush = Img_color->Brush;
		Brush.SetResourceObject(Texture);
		Img_color->SetBrush(Brush);
	
	}
}

void UCustomColorButtonWidget::ChangePawnColor(ECostumeColor color)
{

	switch (color)
	{
	case ECostumeColor::NONE:
		CustomName = "TEST00";

		break;
	case ECostumeColor::PINK:
		CustomName = "TEST01";

		break;
	case ECostumeColor::YELLOW:
		CustomName = "TEST00";

		break;
	case ECostumeColor::BLUE:
		CustomName = "TEST02";

		break;
	default:
		break;

	}
	GetGameInstance<UBaseGameInstance>()->ChangeCostume(GetOwningPlayerPawn(), CustomName);

}

void UCustomColorButtonWidget::SetButtonColor()
{
	//LoadTexture
	//
	switch (Color)
	{
	case ECostumeColor::NONE:
		break;
	case ECostumeColor::PINK:
		SetButtonStyle("/Game/BP/HUD/Custom/Mat/M_PinkColor.M_PinkColor");
		break;
	case ECostumeColor::YELLOW:

		SetButtonStyle("/Game/BP/HUD/Custom/Mat/M_YellowColor.M_YellowColor");
		break;
	case ECostumeColor::BLUE:
		SetButtonStyle("/Game/BP/HUD/Custom/Mat/M_BlueColor.M_BlueColor");
		break;
	default:
		break;

	}
}


