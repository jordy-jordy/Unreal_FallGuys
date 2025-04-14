// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/UI/PlayGameOverWidget.h"
#include "Global/FallGlobal.h"
#include "Mode/01_Play/UI/PlayMainWidget.h"
#include "Mode/01_Play/UI/PlayInGameWidget.h"
#include "Mode/01_Play/UI/PlayResultWidget.h"

// ��������Ʈ �׽�Ʈ
//#include "Mode/01_Play/PlayGameMode.h"
#include "Mode/01_Play/PlayGameState.h"


void UPlayGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ��������Ʈ �׽�Ʈ : 250409 ������ : ����ȭ�� ���� GameState�� �̵�
	APlayGameState* FallState = GetWorld()->GetGameState<APlayGameState>();
	if (FallState)
	{
		FallState->RegisterWidgetDelegate(
			TEXT("GameOver"),
			FWidgetDelegate::CreateUObject(this, &UPlayGameOverWidget::WidgetVisible));
	}
	// ��������Ʈ �׽�Ʈ

	PlayAnimation(GameOverAnim);
	GameOverAnimEvent.BindUFunction(this, FName(FString(TEXT("MoveToResultLevel"))));
	BindToAnimationFinished(GameOverAnim, GameOverAnimEvent);
}

void UPlayGameOverWidget::WidgetVisible()
{
	GetMainWidget()->AllWidgetHidden();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UPlayGameOverWidget::MoveToResultLevel()
{
	UPlayInGameWidget* InGameWidget = Cast<UPlayInGameWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayInGame));
	UPlayResultWidget* PlayResult = Cast<UPlayResultWidget>(GetMainWidget()->FindWidget(EPlayUIType::PlayResult));
	bool IsShowResult = InGameWidget->GetShowResult();

	if (false == IsShowResult)
	{
		SetVisibility(ESlateVisibility::Hidden);
		PlayResult->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		UFallGlobal::SetCanMoveLevel(true);
	}
}
