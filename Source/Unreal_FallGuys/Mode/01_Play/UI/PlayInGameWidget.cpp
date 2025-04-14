// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayInGameWidget.h"
#include "Mode/01_Play/PlayGameState.h"
#include "Mode/01_Play/PlayPlayerState.h"
#include "Mode/01_Play/PlayCharacter.h"
#include "Mode/01_Play/UI/PlayResultWidget.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"


void UPlayInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IsShowResult = false;
}

void UPlayInGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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
		UPlayResultWidget* ResultWidget = Cast<UPlayResultWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayResult));
		ResultWidget->ChangeResources();
	}
}

// юс╫ц
EStagePhase UPlayInGameWidget::CurStagePhase()
{
	APlayGameState* PlayGameState = Cast<APlayGameState>(GetWorld()->GetGameState());

	EStagePhase CurStagePhase = PlayGameState->GetCurStagePhase_STATE();

	return CurStagePhase;
}

void UPlayInGameWidget::ShowResult(bool _Value)
{
	IsShowResult = _Value;
}

