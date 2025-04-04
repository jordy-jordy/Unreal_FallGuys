// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"
#include "PlayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayResultWidget : public UPlayUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();
	
private:
	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* LeftBigAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* LeftSmallAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* RightBigAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* RightSmallAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* TextAnim;
};
