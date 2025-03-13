// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomTopButtonWidget.h"


void UCustomTopButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetButtonTopStyle();
}

void UCustomTopButtonWidget::SetButtonTopStyle()
{
	//LoadTexture
	//

	
	SetButtonStyle(NewTexture,"/Script/Engine.Texture2D'/Game/Resources/UI/Custom/Character/UI_Icon_Bottom_Tanager_Variant01.UI_Icon_Bottom_Tanager_Variant01'");

	/*switch (Type)
	{
	case ECostumeType::NONE:

		break;
	case ECostumeType::TOP:

		break;
	case ECostumeType::BOTTOM:
		break;
	case ECostumeType::MAX:
		break;
	default:
		break;
	}*/


}
