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
	FString GetTimeLimit_Minutes();

	UFUNCTION(BlueprintCallable)
	FString GetTimeLimit_Seconds();

};
