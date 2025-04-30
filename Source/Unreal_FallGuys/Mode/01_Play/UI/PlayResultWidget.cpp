// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayResultWidget.h"
#include "Mode/01_Play/PlayGameState.h"
#include "Global/FallGlobal.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"
#include "Mode/01_Play/UI/PlayInGameWidget.h"
#include "Mode/01_Play/PlayCharacter.h"


void UPlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != ResultAnim)
	{
		ResultAnimFinished.BindDynamic(this, &UPlayResultWidget::FinishedResultWidget);
		BindToAnimationFinished(ResultAnim, ResultAnimFinished);
		PlayAnimation(ResultAnim);
	}

	IsAnimated = false;
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

	ReturnBGMVolume();

	int WholePlayerNum = PlayGameState->GetGameStateFinishPlayer();
	int TargetPlayerNum = PlayGameState->GetGameStateCurFinishPlayer();

	EStageType StageType = PlayGameState->GetLevelType_STATE();

	if (StageType == EStageType::SOLO)
	{
		if ((WholePlayerNum <= TargetPlayerNum) && true == IsAnimated)
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
	else if (StageType == EStageType::TEAM)
	{
		int CurLimitTime = static_cast<int>(UFallGlobal::GetRemainingTime());
		if (0.1f < CurLimitTime)
		{
			UPlayInGameWidget* InGameWidget = Cast<UPlayInGameWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayInGame));
			InGameWidget->ShowResult(true);
			//GetMainWidget()->AllWidgetHidden();
			//GetMainWidget()->SwitchWidget(EPlayUIType::PlayInGame);
		}
	}
}






