// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include <Global/Data/CostumeDataTable.h>
#include <Global/Data/PlayLevelDataTable.h>

#include "GlobalDataTable.generated.h"


USTRUCT(BlueprintType)
struct FDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FDataTableRow() {}
	~FDataTableRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* Resources = nullptr;
};

USTRUCT(BlueprintType)
struct FActorClassTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FActorClassTableRow() {}
	~FActorClassTableRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<class AActor> SpawnClass;
};


/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UGlobalDataTable : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//UFUNCTION(BlueprintCallable)
	//static TSubclassOf<AActor> GetActorClass(UWorld* _World, const FString& _Name);
	static const FCostumeDataRow* GetCostumeData(UWorld* _World, const FString& _Name);
	static const TSoftObjectPtr<UWorld> GetPlayLevelData(UWorld* _World, const FString& _Name);
};
