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
	// 레벨 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	FString Name;

	// 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	TSoftObjectPtr<UWorld> Level;

	// 전환 화면용 레벨 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	UTexture* LevelIMG = nullptr;
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayLevelDataTable : public UObject
{
	GENERATED_BODY()
	
};
