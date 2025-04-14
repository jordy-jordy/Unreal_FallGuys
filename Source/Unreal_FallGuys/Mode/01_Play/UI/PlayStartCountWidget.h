// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"
#include "PlayStartCountWidget.generated.h"


/**
 *
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayStartCountWidget : public UPlayUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetWidgetImage(class UImage* _CountImage, TArray<class UTexture2D*> _ArrTexture, int _Index);

	UFUNCTION(BlueprintCallable)
	void StartCountWidget();

	UFUNCTION()
	void AfterCountWidget();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UTexture2D*> ArrTexture;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* CountAnim;

	FWidgetAnimationDynamicEvent CountWidgetAnimationEvent;
};
