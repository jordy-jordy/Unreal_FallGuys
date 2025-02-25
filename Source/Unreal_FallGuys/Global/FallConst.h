// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FallConst.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UFallConst : public UObject
{
	GENERATED_BODY()

public:

private:
	friend class UBaseGameInstance;
	static FString PlayLevelName;

};
