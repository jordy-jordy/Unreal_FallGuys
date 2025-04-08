// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"
#include "PlayGameOverWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FGameOverWidgetDelegate);

UCLASS()
class UNREAL_FALLGUYS_API UPlayGameOverWidget : public UPlayUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();

	//FGameOverWidgetDelegate GameOverWidgetVisible();

	UFUNCTION(BlueprintCallable)
	void WidgetVisible();

private:
	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* GameOverAnim;
};
