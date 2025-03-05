// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include <Global/Data/GlobalDataTable.h>

#include "BaseGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	friend class UFallGlobal;
	friend class UGlobalDataTable;
	
public:
	UBaseGameInstance();

	// 코스튬 저장
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Costume")
	void SaveSelectedCostume_Implementation(const FString& CostumeName);

	// 저장된 코스튬 반환
	UFUNCTION(BlueprintCallable, Category = "Costume")
	FString GetSelectedCostume() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:


private:
	UFUNCTION(BlueprintCallable, Category = "Server")
	void CServerStart(UWorld* _World, FString _Port);

	UFUNCTION(BlueprintCallable, Category = "Server")
	void CServerConnect(UWorld* _World, FString _IP, FString _Port);

	UPROPERTY(VisibleAnywhere, Category = "Data")
	class UDataTable* DataTables = nullptr;
	class UDataTable* ActorDataTable = nullptr;
	class UDataTable* CostumeDataTable = nullptr;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Costume")
	FString SelectedCostumeName = TEXT("TEST00");
};
