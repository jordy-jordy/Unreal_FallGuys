// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <Global/GlobalEnum.h>

#include "CostumeColorDataTable.generated.h"


USTRUCT(BlueprintType)
struct FCostumeColorDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FCostumeColorDataRow() {}
	~FCostumeColorDataRow() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeColor")
	ECostumeColor Color = ECostumeColor::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CostumeColor")
	USkeletalMesh* CostumeMesh = nullptr;
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UCostumeColorDataTable : public UObject
{
	GENERATED_BODY()
	
};
