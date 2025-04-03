// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"
#include "PlayLevelTagWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayLevelTagWidget : public UPlayUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StartPlayAnim();

	UFUNCTION(BlueprintCallable)
	void ReversePlayAnim();

private:
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* LevelNamePartPanelAnim;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* TagWhitePartAnim;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* LevelExpPartPanelAnim;
};
