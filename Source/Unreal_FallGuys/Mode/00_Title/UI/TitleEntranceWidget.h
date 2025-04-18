// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitleEntranceWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleEntranceWidget : public UTitleUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetTargetStageType(EStageType _TargetStage)
	{
		TagetStage = _TargetStage;
	}

	UFUNCTION(BlueprintCallable)
	EStageType GetTargetStageType()
	{
		return TagetStage;
	}

private:
	EStageType TagetStage;
};
