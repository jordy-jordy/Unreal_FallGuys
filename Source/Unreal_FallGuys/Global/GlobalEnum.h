// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GlobalEnum.generated.h"


UENUM(BlueprintType)
enum class ETeamType : uint8
{
	RED UMETA(DisplayName = "Red"),
	BLUE UMETA(DisplayName = "BLUE"),
	NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class ECostumeColor : uint8
{
	DEFAULT UMETA(DisplayName = "Default"),
	PINK UMETA(DisplayName = "Pink"),
	YELLOW UMETA(DisplayName = "Yellow"),
	BLUE UMETA(DisplayName = "Blue"),
	RED UMETA(DisplayName = "Red"),
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
enum class ETitleUIType : uint8
{
	TitleHome UMETA(DisplayName = "TitleHome"),
	TitleCustom UMETA(DisplayName = "TitleCustom"),
	TitleEntrance UMETA(DisplayName = "TitleEntrance"),
	CustomeInven UMETA(DisplayName = "CustomeInven"),
	InvenBtn UMETA(DisplayName = "InvenBtn"),
	TitleIPPort UMETA(DisplayName = "TitleIPPort"),
	TitleName UMETA(DisplayName = "TitleName"),
	TitleMenu UMETA(DisplayName = "TitleMenu"),
	MAX UMETA(DisplayName = "MAX"),
};

UENUM(BlueprintType)
enum class EPlayUIType : uint8
{
	PlayStandby UMETA(DisplayName = "PlayStandby"),
	PlayInGame UMETA(DisplayName = "PlayInGame"),
	PlayLevelTag UMETA(DisplayName = "PlayLevelTag"),
	PlayStartCount UMETA(DisplayName = "PlayStartCount"),
	PlayReturnHome UMETA(DisplayName = "PlayReturnHome"),
	PlayResult UMETA(DisplayName = "PlayResult"),
	PlaySpectatorResult UMETA(DisplayName = "PlaySpectatorResult"),
	PlayGameOver UMETA(DisplayName = "PlayGameOver"),
	PlayClearCount UMETA(DisplayName = "PlayClearCount"),
	PlayScore UMETA(DisplayName = "PlayScore"),
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
