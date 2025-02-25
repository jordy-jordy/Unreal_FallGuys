// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:


private:
	friend class UFallGlobal;
	UFUNCTION(BlueprintCallable, Category = "Server")
	void CServerStart(UWorld* _World, FString _Port);

	UFUNCTION(BlueprintCallable, Category = "Server")
	void CServerConnect(UWorld* _World, FString _IP, FString _Port);

};
