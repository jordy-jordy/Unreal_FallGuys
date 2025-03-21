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
	JumpStart UMETA(DisplayName = "JumpStart"),
	JumpMidAir UMETA(DisplayName = "JumpMidAir"),
	Fall UMETA(DisplayName = "Fall"),
	DiveStart UMETA(DisplayName = "DiveStart"),
	DiveLoop UMETA(DisplayName = "DiveLoop"),
	DiveLand UMETA(DisplayName = "DiveLand"),
	DiveGetUp UMETA(DisplayName = "DiveGetUp"),
	GrabIdle UMETA(DisplayName = "GrabIdle"),
	GrabRun UMETA(DisplayName = "GrabRun"),
	CarryIdle UMETA(DisplayName = "CarryIdle"),
	CarryRun UMETA(DisplayName = "CarryRun"),
	MAX UMETA(DisplayName = "MAX"),
};

//LMH
UENUM(BlueprintType)
enum class ERaceOverAnimation : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Falling UMETA(DisplayName = "Falling")
};
/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayEnum : public UObject
{
	GENERATED_BODY()
	
};
