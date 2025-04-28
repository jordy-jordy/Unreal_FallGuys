// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleCustomizingMainWidget.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"


void UTitleCustomizingMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurInvenType = ECustomInvenType::Custom;
	CurInvenIndex = 0;
}

void UTitleCustomizingMainWidget::AddAllCustomInvens()
{
	TMultiMap<ETitleUIType, UTitleUserWidget*>& AllWidget = GetMainWidget()->GetAllWidgets();

	for (TPair<ETitleUIType, UTitleUserWidget*> Pair : AllWidget)
	{
		if (Pair.Key == ETitleUIType::InvenBtn)
		{
			ECustomInvenType Type;
			UTitleUserWidget* CurWidget = Pair.Value;
			FString WidgetName = CurWidget->GetClass()->GetName();

			if (WidgetName.Contains(FString("Custom")))
			{
				Type = ECustomInvenType::Custom;
			}
			else if (WidgetName.Contains(FString("Animation")))
			{
				Type = ECustomInvenType::Animation;
			}
			else
			{
				Type = ECustomInvenType::Interface;
			}

			ArrCustomInvens.Emplace(Type, Pair.Value);
		}
	}
}

void UTitleCustomizingMainWidget::ChangeType_Left()
{
	CurInvenIndex -= 1;
	if (CurInvenIndex < 0)
	{
		CurInvenIndex = 0;
	}

	switch (CurInvenIndex)
	{
	case 0:
	{
		CurInvenType = ECustomInvenType::Custom;
		break;
	}
	case 1:
	{
		CurInvenType = ECustomInvenType::Animation;
		break;
	}
	case 2:
	{
		CurInvenType = ECustomInvenType::Interface;
		break;
	}
	default:
		break;
	}
}

void UTitleCustomizingMainWidget::ChangeType_Right()
{
	CurInvenIndex += 1;
	if (CurInvenIndex > ArrCustomInvens.Num() - 1)
	{
		CurInvenIndex = ArrCustomInvens.Num() - 1;
	}

	switch (CurInvenIndex)
	{
	case 0:
	{
		CurInvenType = ECustomInvenType::Custom;
		break;
	}
	case 1:
	{
		CurInvenType = ECustomInvenType::Animation;
		break;
	}
	case 2:
	{
		CurInvenType = ECustomInvenType::Interface;
		break;
	}
	default:
		break;
	}
}