// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Global/GlobalEnum.h"

#include "TeamPlayLevelDataTable.generated.h"


USTRUCT(BlueprintType)
struct FTeamPlayLevelDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FTeamPlayLevelDataRow() {}
	~FTeamPlayLevelDataRow() {}

public:
	// 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	TSoftObjectPtr<UWorld> Level;

	// 레벨 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	FString Name;

	// 레벨 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	EStageType LevelType = EStageType::TEAM;

	// 플레이 제한 시간 - 패키징때 쓰는 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	float StageLimitTime = 120.0f;

	// 전환 화면용 레벨 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	UTexture2D* LevelIMG = nullptr;

	// 전환 화면용 레벨 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	UTexture2D* LevelTagIMG = nullptr;

	// 플레이 가이드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	FString PlayGuide;

	// 플레이 화면에서 보여주는 목표
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	FString GoalGuide;
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTeamPlayLevelDataTable : public UObject
{
	GENERATED_BODY()
	
};
