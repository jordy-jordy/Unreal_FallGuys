// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"
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
	void AnimationDebug();
	
private:
	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* BarPatternAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* UpperStrokeAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* LowerStroke1Anim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* LowerStroke2Anim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* RightBigAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* LeftBigAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* RIghtSmallAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* LeftSmallAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* TextAnim;
};
