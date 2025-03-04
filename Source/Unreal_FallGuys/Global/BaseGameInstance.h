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

	UPROPERTY()
	class UPlayerClothManager* PlayerClothManager;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Costume")
	void SelectPlayerCloth(int32 PlayerID, FString ClothID);


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


};
