// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GlobalEnum.generated.h"


// 코스튬 색상
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
enum class EUIType : uint8
{
	TitleHome UMETA(DisplayName = "TitleHome"),
	TitleCustom UMETA(DisplayName = "TitleCustom"),
	TitleEntrance UMETA(DisplayName = "TitleEntrance"),
	CustomInven UMETA(DisplayName = "CustomInven"),
	TitleIPPort UMETA(DisplayName = "TitleIPPort"),
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
