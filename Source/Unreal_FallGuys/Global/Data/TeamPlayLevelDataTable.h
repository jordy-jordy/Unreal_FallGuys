// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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

	// 제한 시간 유무
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	bool UseLimitTime = false;

	// Stage 플레이 제한 시간 - 패키징때 쓰는 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	float StageLimitTime = 60.0f;

	// 전환 화면용 레벨 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEAM LEVEL DATA")
	UTexture2D* LevelIMG = nullptr;

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
