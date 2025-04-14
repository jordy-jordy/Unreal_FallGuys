// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayResultWidget.h"
#include "Mode/01_Play/PlayGameState.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"
#include "Mode/01_Play/UI/PlayInGameWidget.h"
#include "Mode/01_Play/PlayCharacter.h"


void UPlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != ResultAnim)
	{
		ResultAnimFinished.BindUFunction(this, FName(FString(TEXT("FinishedResultWidget"))));
		BindToAnimationFinished(ResultAnim, ResultAnimFinished);

		PlayAnimation(ResultAnim);
	}
}

void UPlayResultWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APlayCharacter* PlayCharacter = Cast<APlayCharacter>(GetOwningPlayerPawn());
	if (nullptr == PlayCharacter)
	{
		return;
	}

	APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayCharacter->GetPlayerState());
	if (nullptr == PlayPlayerState)
	{
		return;
	}

	if (EPlayerStatus::SUCCESS == PlayPlayerState->GetPlayerStateStatus())
	{
		ChangeResources();
	}
}

void UPlayResultWidget::FinishedResultWidget()
{
	APlayGameState* PlayGameState = Cast<APlayGameState>(GetWorld()->GetGameState());
	if (nullptr == PlayGameState)
	{
		return;
	}

	int WholePlayerNum = PlayGameState->GetGameStateFinishPlayer();
	int TargetPlayerNum = PlayGameState->GetGameStateCurFinishPlayer();

	if (WholePlayerNum <= TargetPlayerNum)
	{
		UFallGlobal::SetCanMoveLevel(true);
	}
	else if (WholePlayerNum > TargetPlayerNum)
	{
		UPlayInGameWidget* InGameWidget = Cast<UPlayInGameWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayInGame));
		InGameWidget->ShowResult(true);
		GetMainWidget()->AllWidgetHidden();
		GetMainWidget()->SwitchWidget(EPlayUIType::PlayInGame);
	}
}






