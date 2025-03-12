// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceDataTable.generated.h"


USTRUCT(BlueprintType)
struct FResourceDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FResourceDataRow() {}
	~FResourceDataRow() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	FString Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	UStaticMesh* Mesh = nullptr;
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UResourceDataTable : public UObject
{
	GENERATED_BODY()
	
};
