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
	// 코스튬 컬러 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COSTUME COLOR")
	FString Name = TEXT("");

	// 코스튬 컬러 이넘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COSTUME COLOR")
	ECostumeColor Color = ECostumeColor::NONE;

	// 코스튬 스켈레탈 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COSTUME COLOR")
	USkeletalMesh* CostumeMesh = nullptr;

	// 코스튬 컬러 머티리얼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COSTUME COLOR")
	UMaterialInterface* ColorUIMaterial = nullptr;
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UCostumeColorDataTable : public UObject
{
	GENERATED_BODY()
	
};
