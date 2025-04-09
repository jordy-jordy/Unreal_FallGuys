// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayInGameWidget.h"
#include "Mode/01_Play/PlayGameState.h"



// юс╫ц
EStagePhase UPlayInGameWidget::CurStagePhase()
{
	APlayGameState* PlayGameState = Cast<APlayGameState>(GetWorld()->GetGameState());

	EStagePhase CurStagePhase = PlayGameState->GetCurStagePhase();

	return CurStagePhase;
}

