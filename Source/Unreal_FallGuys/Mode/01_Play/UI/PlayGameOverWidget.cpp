// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayGameOverWidget.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"

// 델리게이트 테스트
#include "Mode/01_Play/PlayGameMode.h"


void UPlayGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 델리게이트 테스트
	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr == PlayGameMode)
	{
		return;
	}
	else
	{
		PlayGameMode->RegisterWidgetDelegate(
			FGameOverWidgetDelegate::CreateUObject(this, &UPlayGameOverWidget::WidgetVisible)
		);
	}
	// 델리게이트 테스트

	PlayAnimation(GameOverAnim);
}

//FGameOverWidgetDelegate UPlayGameOverWidget::GameOverWidgetVisible()
//{
//	return FGameOverWidgetDelegate::CreateUObject(this, &UPlayGameOverWidget::WidgetVisible);
//}

void UPlayGameOverWidget::WidgetVisible()
{
	GetMainWidget()->AllWidgetHidden();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}