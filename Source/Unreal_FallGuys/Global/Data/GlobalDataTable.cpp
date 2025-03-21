// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Data/GlobalDataTable.h"

#include <Global/BaseGameInstance.h>
#include <Unreal_FallGuys.h>


const FCostumeDataRow* UGlobalDataTable::GetCostumeData(UWorld* _World, const FString& _Name)
{
	UBaseGameInstance* Inst = _World->GetGameInstance<UBaseGameInstance>();

	if (nullptr == Inst->CostumeDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == Inst->CostumeDataTable)"), __FUNCTION__, __LINE__);
	}

	FCostumeDataRow* Data = Inst->CostumeDataTable->FindRow<FCostumeDataRow>(*_Name, nullptr);

	if (nullptr == Data)
	{
		return nullptr;
	}

	return Data;
}

const FCostumeColorDataRow* UGlobalDataTable::GetCostumeColorData(UWorld* _World, const FString& _Name)
{
	UBaseGameInstance* Inst = _World->GetGameInstance<UBaseGameInstance>();

	if (nullptr == Inst->CostumeColorDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == Inst->CostumeColorDataTable)"), __FUNCTION__, __LINE__);
	}

	FCostumeColorDataRow* Data = Inst->CostumeColorDataTable->FindRow<FCostumeColorDataRow>(*_Name, nullptr);

	if (nullptr == Data)
	{
		return nullptr;
	}

	return Data;
}

const FPlayLevelDataRow* UGlobalDataTable::GetPlayLevelData(UWorld* _World, const FString& _Name)
{
	UBaseGameInstance* Inst = _World->GetGameInstance<UBaseGameInstance>();

	if (nullptr == Inst->PlayLevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == Inst->PlayLevelDataTable)"), __FUNCTION__, __LINE__);
	}

	FPlayLevelDataRow* Data = Inst->PlayLevelDataTable->FindRow<FPlayLevelDataRow>(*_Name, nullptr);

	if (nullptr == Data)
	{
		return nullptr;
	}

	return Data;
}

const FResourceDataRow* UGlobalDataTable::GetResourceData(UWorld* _World, const FString& _Name)
{
	UBaseGameInstance* Inst = _World->GetGameInstance<UBaseGameInstance>();

	if (nullptr == Inst->ResourceDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == Inst->ResourceDataTable)"), __FUNCTION__, __LINE__);
	}

	FResourceDataRow* Data = Inst->ResourceDataTable->FindRow<FResourceDataRow>(*_Name, nullptr);

	if (nullptr == Data)
	{
		return nullptr;
	}

	return Data;
}


//TSubclassOf<AActor> UGlobalDataTable::GetActorClass(UWorld* _World, const FString& _Name)
//{
//	UBaseGameInstance* Inst = _World->GetGameInstance<UBaseGameInstance>();
//
//	if (nullptr == Inst->ActorDataTable)
//	{
//		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == ActorDataTable)"), __FUNCTION__, __LINE__);
//	}
//
//	FActorClassTableRow* Data = Inst->ActorDataTable->FindRow<FActorClassTableRow>(*_Name, nullptr);
//
//	if (nullptr == Data)
//	{
//		return nullptr;
//	}
//
//	return Data->SpawnClass;
//}
