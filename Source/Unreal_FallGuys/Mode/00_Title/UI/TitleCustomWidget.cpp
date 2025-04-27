// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleCustomWidget.h"


void UTitleCustomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurCustomType = ECustomType::Color;
	CurIndex = 0;
	CurColorIndex = 0;
	CurColor = ArrColor[0];

}

void UTitleCustomWidget::ChangeType_Left()
{
	CurIndex -= 1;

	if (CurIndex < 0)
	{
		CurIndex = 0;
	}

	switch (CurIndex)
	{
	case 0:
	{
		CurCustomType = ECustomType::Color;
		break;
	}
	case 1:
	{
		CurCustomType = ECustomType::Pattern;
		break;
	}
	case 2:
	{
		CurCustomType = ECustomType::Emotion;
		break;
	}
	case 3:
	{
		CurCustomType = ECustomType::Top;
		break;
	}
	case 4:
	{
		CurCustomType = ECustomType::Bottom;
		break;
	}
	default:
		break;
	}
}

void UTitleCustomWidget::ChangeType_Right()
{
	CurIndex += 1;

	if (CurIndex > AllSwitcherNum - 1)
	{
		CurIndex = AllSwitcherNum - 1;
	}

	switch (CurIndex)
	{
	case 0:
	{
		CurCustomType = ECustomType::Color;
		break;
	}
	case 1:
	{
		CurCustomType = ECustomType::Pattern;
		break;
	}
	case 2:
	{
		CurCustomType = ECustomType::Emotion;
		break;
	}
	case 3:
	{
		CurCustomType = ECustomType::Top;
		break;
	}
	case 4:
	{
		CurCustomType = ECustomType::Bottom;
		break;
	}
	default:
		break;
	}
}

void UTitleCustomWidget::ChangeColorIndex_Left()
{
	CurColorIndex -= 1;

	if (CurColorIndex < 0)
	{
		CurColorIndex = 0;
	}

	CurColor = ArrColor[CurColorIndex];
}

void UTitleCustomWidget::ChangeColorIndex_Right()
{
	CurColorIndex += 1;

	if (CurColorIndex > ArrColor.Num() - 1)
	{
		CurColorIndex = ArrColor.Num() - 1;
	}

	CurColor = ArrColor[CurColorIndex];
}

void UTitleCustomWidget::CurColorSetting()
{
	for (int i = 0; i < ArrColor.Num(); i++)
	{
		if (ArrColor[i] == CurColor)
		{
			CustomColorMove();
		}
		else
		{
			CustomColorReturn();
		}
	}
}