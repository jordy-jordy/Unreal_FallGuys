// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FallGlobal.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UFallGlobal : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void AssetPackagePath(UClass* _Class, const FString& _AssetName, FString& _Path);

	UFUNCTION(BlueprintCallable)
	static TArray<FAssetData> AssetsPath(UClass* _Class);
	
	UFUNCTION(BlueprintCallable)
	static void ServerStart(UObject* _Object, FString _Port);
	
	UFUNCTION(BlueprintCallable)
	static void ServerConnect(UObject* _Object, FString _IP, FString _Port);

	// Pawn의 코스튬 변경
	UFUNCTION(BlueprintCallable)
	static void ChangeCurCostume(APawn* _Pawn, const FString& _CostumeName);
	
	// 저장된 코스튬의 이름 반환
	UFUNCTION(BlueprintCallable)
	static FString GetSelectedCostume(APawn* _Pawn);

	// 저장된 코스튬의 스켈레탈 메시 반환
	UFUNCTION(BlueprintCallable)
	static USkeletalMesh* GetCostumeMesh(APawn* _Pawn, const FString& _MeshName = TEXT("NULL"));


};
