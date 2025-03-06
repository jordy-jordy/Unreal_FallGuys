// Fill out your copyright notice in the Description page of Project Settings.

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
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UGlobalEnum : public UObject
{
	GENERATED_BODY()
	
};
