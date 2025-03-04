// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Data/GlobalDataTable.h"

#include <Global/BaseGameInstance.h>
#include <Unreal_FallGuys.h>


TSubclassOf<AActor> UGlobalDataTable::GetActorClass(UWorld* _World, const FString& _Name)
{
	UBaseGameInstance* Inst = _World->GetGameInstance<UBaseGameInstance>();

	if (nullptr == Inst->ActorDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == ActorDataTable)"), __FUNCTION__, __LINE__);
	}

	FActorClassTableRow* Data = Inst->ActorDataTable->FindRow<FActorClassTableRow>(*_Name, nullptr);

	if (nullptr == Data)
	{
		return nullptr;
	}

	return Data->SpawnClass;
}

const FCostumeDataRow* UGlobalDataTable::GetCostumeData(UWorld* _World, const FString& _Name)
{
	UBaseGameInstance* Inst = _World->GetGameInstance<UBaseGameInstance>();

	if (nullptr == Inst->CostumeDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == Inst->ItemDataTable)"), __FUNCTION__, __LINE__);
	}

	FCostumeDataRow* Data = Inst->CostumeDataTable->FindRow<FCostumeDataRow>(*_Name, nullptr);

	if (nullptr == Data)
	{
		return nullptr;
	}

	return Data;
}