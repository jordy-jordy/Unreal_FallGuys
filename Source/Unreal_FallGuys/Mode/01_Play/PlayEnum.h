// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayEnum.generated.h"

UENUM(BlueprintType)
enum class EPlayerAnimation : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Run UMETA(DisplayName = "Run"),
	Jump UMETA(DisplayName = "Jump"),
	MAX UMETA(DisplayName = "MAX"),
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayEnum : public UObject
{
	GENERATED_BODY()
	
};
