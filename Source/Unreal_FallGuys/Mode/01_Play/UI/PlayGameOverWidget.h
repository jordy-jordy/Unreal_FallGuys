// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"

// ��������Ʈ �׽�Ʈ
#include "Global/FallGlobal.h"

#include "PlayGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayGameOverWidget : public UPlayUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void WidgetVisible();

	UFUNCTION()
	void MoveToResultLevel();

private:
	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* GameOverAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* GameOverAnim_Clear;

	FWidgetAnimationDynamicEvent GameOverAnimEvent;
	FWidgetAnimationDynamicEvent GameOverAnim_ClearEvent;
};
