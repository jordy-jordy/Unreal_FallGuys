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

void UPlayStartCountWidget::CountDownWidget()
{
	// FString ImagePath = TEXT("");


	//UPlayUserWidget* CountWidget = GetMainWidget()->FindWidget(EPlayUIType::PlayStartCount);

	//// UTexture2D* Image = LoadObject<UTexture2D>(nullptr, _ImagePath);

	//CountImage = Cast<UImage>(CountWidget->GetWidgetFromName(TEXT("CountImage")));

	//FVector2D ImageSize = CountImage->GetBrush().ImageSize;

	//CountImage->SetBrushFromTexture(ArrTexture[static_cast<int>(_CountTime)]);
	//CountImage->SetDesiredSizeOverride(ImageSize);

	//PlayAnimation(CountAnim);
}

