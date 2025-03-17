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
	// 리소스 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RESOURCE")
	FString Name;

	// 리소스 컬러
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RESOURCE")
	FString Color;

	// 리소스 스테틱 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RESOURCE")
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
