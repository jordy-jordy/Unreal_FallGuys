// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"
#include "PlayScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayScoreWidget : public UPlayUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();

	// �ӽ�
	UFUNCTION(BlueprintCallable)
	int GetTimeLimit_Minutes();

	// �ӽ�
	UFUNCTION(BlueprintCallable)
	int GetTimeLimit_Seconds();

	//UFUNCTION(BlueprintCallable)
	//float GetLimitTime();

	//UFUNCTION(BlueprintCallable)
	//void LimitTimeDown();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	float CurLimitTime = 0.0f;

	//FTimerHandle TimerHandle;
	//FTimerDynamicDelegate TimerDelegate;
};
