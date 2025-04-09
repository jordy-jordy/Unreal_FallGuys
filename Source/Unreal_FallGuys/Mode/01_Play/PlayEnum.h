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


UENUM(BlueprintType)
enum class ERhinoAnimation : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Run UMETA(DisplayName = "Run"),
	Antic UMETA(DisplayName = "Antic"),
	Charge UMETA(DisplayName = "Charge"),
	Stop UMETA(DisplayName = "Stop"),
	MAX UMETA(DisplayName = "MAX"),
};

//LMH
UENUM(BlueprintType)
enum class ERaceOverAnimation : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Falling UMETA(DisplayName = "Falling")
};

// 이현정 : 스테이지 관리
UENUM(BlueprintType)
enum class EStagePhase : uint8
{
	STAGE_1			UMETA(DisplayName = "STAGE 1"),
	STAGE_1_RESULT	UMETA(DisplayName = "STAGE 1 RESULT"),
	STAGE_2			UMETA(DisplayName = "STAGE 2"),
	STAGE_2_RESULT	UMETA(DisplayName = "STAGE 2 RESULT"),
	STAGE_3			UMETA(DisplayName = "STAGE 3"),
	STAGE_3_RESULT	UMETA(DisplayName = "STAGE 3 RESULT"),
	FINISHED		UMETA(DisplayName = "FINISHED")
};


/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayEnum : public UObject
{
	GENERATED_BODY()
	
};
