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
	// 코스튬 타입 :: 상, 하의
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COSTUME DATA")
	ECostumeType Type = ECostumeType::NONE;

	// 코스튬 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COSTUME DATA")
	FVector CostumePos = FVector::ZeroVector;

	// 코스튬 피봇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COSTUME DATA")
	FVector CostumePivot = FVector::ZeroVector;

	// 코스튬 스테틱 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COSTUME DATA")
	UStaticMesh* CostumeMesh = nullptr;

	// 코스튬 UI용 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COSTUME DATA")
	UTexture2D* CostumeIMG = nullptr;

};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UCostumeDataTable : public UObject
{
	GENERATED_BODY()
};
