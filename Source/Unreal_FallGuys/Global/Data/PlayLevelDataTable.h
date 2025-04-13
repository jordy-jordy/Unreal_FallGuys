// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Global/GlobalEnum.h"

#include "PlayLevelDataTable.generated.h"


USTRUCT(BlueprintType)
struct FPlayLevelDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FPlayLevelDataRow() {}
	~FPlayLevelDataRow() {}

public:
	// 레벨을 사용할 것인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	bool UseLevel = true;

	// 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	TSoftObjectPtr<UWorld> Level = nullptr;

	// 레벨 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	FString Name = TEXT("");

	// 레벨 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	EStageType LevelType = EStageType::SOLO;

	// 스테이지 종료 기준
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	EPlayerStatus EndCondition = EPlayerStatus::NONE;

	// 전환 화면용 레벨 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	UTexture2D* LevelIMG = nullptr;

	// 전환 화면용 레벨 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	UTexture2D* LevelTagIMG = nullptr;

	// 플레이 가이드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	FString PlayGuide = TEXT("");

	// 플레이 화면에서 보여주는 목표
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEVEL DATA")
	FString GoalGuide = TEXT("");
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayLevelDataTable : public UObject
{
	GENERATED_BODY()
	
};
