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




};
