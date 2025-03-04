// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CostumeDataTable.generated.h"

USTRUCT(BlueprintType)
struct FCostumeDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FCostumeDataRow() {};
	~FCostumeDataRow() {};

public:



};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UCostumeDataTable : public UObject
{
	GENERATED_BODY()
	
};
