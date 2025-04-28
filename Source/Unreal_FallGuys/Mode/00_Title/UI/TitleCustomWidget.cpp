// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleCustomWidget.h"


void UTitleCustomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurCustomType = ECustomType::Color;
	CurIndex = 0;

	// CustomColor
	CurColorIndex = 0;
	if (false == ArrColor.IsEmpty())
	{
		CurColor = ArrColor[0];
	}

	// CustomTop
	CurTopIndex = 0;
	if (false == ArrTop.IsEmpty())
	{
		CurTop = ArrTop[0];
	}

	// CustomBottom
	CurBottomIndex = 0;
	if (false == ArrBottom.IsEmpty())
	{
		CurBottom = ArrBottom[0];
	}
}

#pragma region CustomMenu
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
#pragma endregion

#pragma region CustomColor
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
#pragma endregion

#pragma region CustomTop
void UTitleCustomWidget::ChangeTopIndex_Up()
{
	CurTopIndex -= 5;

	if (CurTopIndex < 0)
	{
		CurTopIndex = 0;
	}

	CurTop = ArrTop[CurTopIndex];
}

void UTitleCustomWidget::ChangeTopIndex_Down()
{
	CurTopIndex += 5;

	if (CurTopIndex > ArrTop.Num() - 1)
	{
		CurTopIndex = ArrTop.Num() - 1;
	}

	CurTop = ArrTop[CurTopIndex];
}

void UTitleCustomWidget::ChangeTopIndex_Left()
{
	CurTopIndex -= 1;

	if (CurTopIndex < 0)
	{
		CurTopIndex = 0;
	}

	CurTop = ArrTop[CurTopIndex];
}

void UTitleCustomWidget::ChangeTopIndex_Right()
{
	CurTopIndex += 1;

	if (CurTopIndex > ArrTop.Num() - 1)
	{
		CurTopIndex = ArrTop.Num() - 1;
	}

	CurTop = ArrTop[CurTopIndex];
}

void UTitleCustomWidget::CurTopSetting()
{
	for (int i = 0; i < ArrTop.Num(); i++)
	{
		if (ArrTop[i] == CurTop)
		{
			CustomTopMove();
		}
		else
		{
			CustomTopReturn();
		}
	}
}
#pragma endregion

#pragma region CustomBottom
void UTitleCustomWidget::ChangeBottomIndex_Up()
{
	CurBottomIndex -= 5;

	if (CurBottomIndex < 0)
	{
		CurBottomIndex = 0;
	}

	CurBottom = ArrBottom[CurBottomIndex];
}

void UTitleCustomWidget::ChangeBottomIndex_Down()
{
	CurBottomIndex += 5;

	if (CurBottomIndex > ArrBottom.Num() - 1)
	{
		CurBottomIndex = ArrBottom.Num() - 1;
	}

	CurBottom = ArrBottom[CurBottomIndex];
}

void UTitleCustomWidget::ChangeBottomIndex_Left()
{
	CurBottomIndex -= 1;

	if (CurBottomIndex < 0)
	{
		CurBottomIndex = 0;
	}

	CurBottom = ArrBottom[CurBottomIndex];
}

void UTitleCustomWidget::ChangeBottomIndex_Right()
{
	CurBottomIndex += 1;

	if (CurBottomIndex > ArrBottom.Num() - 1)
	{
		CurBottomIndex = ArrBottom.Num() - 1;
	}

	CurBottom = ArrBottom[CurBottomIndex];
}

void UTitleCustomWidget::CurBottomSetting()
{
	for (int i = 0; i < ArrBottom.Num(); i++)
	{
		if (ArrBottom[i] == CurBottom)
		{
			CustomBottomMove();
		}
		else
		{
			CustomBottomReturn();
		}
	}
}
#pragma endregion
