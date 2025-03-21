﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GlobalEnum.generated.h"


UENUM(BlueprintType)
enum class ECostumeColor : uint8
{
	NONE UMETA(DisplayName = "None"),
	PINK UMETA(DisplayName = "Pink"),
	YELLOW UMETA(DisplayName = "Yellow"),
	BLUE UMETA(DisplayName = "Blue"),
	MAX UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ECostumeType : uint8
{
	NONE UMETA(DisplayName = "None"),
	TOP UMETA(DisplayName = "Top"),
	BOTTOM UMETA(DisplayName = "Bottom"),
	MAX UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	NONE UMETA(DisplayName = "None"),
	DEFAULT UMETA(DisplayName = "Default"),
	FAIL UMETA(DisplayName = "Fail"),
	SUCCESS UMETA(DisplayName = "Success"),
	MAX UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EUIType : uint8
{
	TitleHome UMETA(DisplayName = "TitleHome"),
	TitleCustom UMETA(DisplayName = "TitleCustom"),
	TitleEntrance UMETA(DisplayName = "TitleEntrance"),
	CustomInven UMETA(DisplayName = "CustomInven"),
	TitleIPPort UMETA(DisplayName = "TitleIPPort"),
	TitleName UMETA(DisplayName = "TitleName"),
	TitleMenu UMETA(DisplayName = "TitleMenu"),
	MAX UMETA(DisplayName = "MAX"),
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UGlobalEnum : public UObject
{
	GENERATED_BODY()
	
};
