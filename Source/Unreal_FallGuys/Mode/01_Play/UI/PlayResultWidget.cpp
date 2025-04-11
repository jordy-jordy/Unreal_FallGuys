// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayResultWidget.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"
#include "Mode/01_Play/UI/PlayInGameWidget.h"


void UPlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != ResultAnim)
	{
		ResultAnimEvent.BindUFunction(this, FName(FString(TEXT("ResultWidget"))));
		BindToAnimationFinished(ResultAnim, ResultAnimEvent);

		PlayAnimation(ResultAnim);
	}
}

void UPlayResultWidget::ResultWidget()
{
	UPlayInGameWidget* InGameWidget = Cast<UPlayInGameWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayInGame));
	InGameWidget->ShowResult(true);
	GetMainWidget()->AllWidgetHidden();
	GetMainWidget()->SwitchWidget(EPlayUIType::PlayInGame);
}






