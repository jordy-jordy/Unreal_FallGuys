// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayInGameWidget.h"
#include "Mode/01_Play/PlayGameState.h"


void UPlayInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IsShowResult = false;
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

