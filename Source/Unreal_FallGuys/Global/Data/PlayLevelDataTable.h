// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayLevelDataTable.generated.h"


USTRUCT(BlueprintType)
struct FPlayLevelDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FPlayLevelDataRow() {}
	~FPlayLevelDataRow() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TSoftObjectPtr<UWorld> Level;  
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayLevelDataTable : public UObject
{
	GENERATED_BODY()
	
};
