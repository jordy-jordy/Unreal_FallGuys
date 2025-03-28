// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayStartCountWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"


void UPlayStartCountWidget::SetWidgetImage(const TCHAR* _ImagePath)
{
	UPlayUserWidget* CountWidget = GetMainWidget()->FindWidget(EPlayUIType::PlayStartCount);

	// TEXT("/Script/Engine.Texture2D'/Game/Resources/UI/01_Play/Textures/Start/T_Count2.T_Count2'")
	UTexture2D* Image = LoadObject<UTexture2D>(nullptr, _ImagePath);

	CountImage = Cast<UImage>(CountWidget->GetWidgetFromName(TEXT("CountImage")));

	FVector2D ImageSize = CountImage->GetBrush().ImageSize;

	CountImage->SetBrushFromTexture(Image);
	CountImage->SetDesiredSizeOverride(ImageSize);
}

void UPlayStartCountWidget::CountDownWidget(float _CountTime)
{
	FString ImagePath = TEXT("");

	if (_CountTime == 1.0f)
	{
		ImagePath = TEXT("/Script/Engine.Texture2D'/Game/Resources/UI/01_Play/Textures/Start/T_Count1.T_Count1'");
	}
	if (_CountTime == 2.0f)
	{
		ImagePath = TEXT("/Script/Engine.Texture2D'/Game/Resources/UI/01_Play/Textures/Start/T_Count2.T_Count2'");
	}
	if (_CountTime == 3.0f)
	{
		ImagePath = TEXT("/Script/Engine.Texture2D'/Game/Resources/UI/01_Play/Textures/Start/T_Count3.T_Count3'");
	}
	if (_CountTime == 0.0f)
	{
		ImagePath = TEXT("/Script/Engine.Texture2D'/Game/Resources/UI/01_Play/Textures/Start/T_GO.T_GO'");
	}

	SetWidgetImage(*ImagePath);
	PlayAnimation(CountAnim);
}

