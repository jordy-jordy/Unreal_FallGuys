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
	//UFUNCTION(BlueprintCallable)
	void SetWidgetImage(const TCHAR* _ImagePath);

	UFUNCTION(BlueprintCallable)
	void CountDownWidget(float _CountTime);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UImage* CountImage = nullptr;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* CountAnim;
};
