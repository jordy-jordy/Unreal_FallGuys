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

	UFUNCTION(BlueprintCallable)
	FString GetTimeLimit_Minutes(FString _Purpose);

	UFUNCTION(BlueprintCallable)
	FString GetTimeLimit_Seconds(FString _Purpose);

	UFUNCTION(BlueprintCallable)
	FString GetLimitTrigger_Minutes(int _Value);

	UFUNCTION(BlueprintCallable)
	FString GetLimitTrigger_Seconds(int _Value);

private:
	FString MText;
	FString SText;
};
