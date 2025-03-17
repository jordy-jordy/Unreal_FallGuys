// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <Global/GlobalEnum.h>

#include "CostumeDataTable.generated.h"


USTRUCT(BlueprintType)
struct FCostumeDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FCostumeDataRow() {}
	~FCostumeDataRow() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeData")
	ECostumeType Type = ECostumeType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeData")
	FVector CostumePos = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeData")
	FVector CostumePivot = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeData")
	UStaticMesh* CostumeMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeData")
	UTexture* CostumeIMG = nullptr;

};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UCostumeDataTable : public UObject
{
	GENERATED_BODY()
};
