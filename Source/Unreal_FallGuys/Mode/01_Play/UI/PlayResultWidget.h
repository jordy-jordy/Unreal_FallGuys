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

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION()
	void FinishedResultWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeResources();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSpectatorView(bool _Value);

	bool GetAnimatedStatus()
	{
		return IsAnimated;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void ReturnBGMVolume();

private:
	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* ResultAnim;

	FWidgetAnimationDynamicEvent ResultAnimFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	bool IsAnimated = false;
};
