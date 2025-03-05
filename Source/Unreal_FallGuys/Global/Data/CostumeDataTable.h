// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CostumeDataTable.generated.h"


USTRUCT(BlueprintType)
struct FCostumeDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FCostumeDataRow() {}
	~FCostumeDataRow() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeData")
	USkeletalMesh* CostumeMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeData")
	FVector CostumePos = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeData")
	FVector CostumePivot = FVector::ZeroVector;

};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UCostumeDataTable : public UObject
{
	GENERATED_BODY()
	
};
