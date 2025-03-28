// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"
#include "PlayStandbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayStandbyWidget : public UPlayUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FString GetCurLevelGuide();
	
private:
};
