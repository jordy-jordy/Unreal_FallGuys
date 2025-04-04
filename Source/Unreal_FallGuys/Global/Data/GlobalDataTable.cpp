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

// 레벨의 에셋 이름으로 데이터 조회
const FPlayLevelDataRow* UGlobalDataTable::FindPlayLevelDataByAssetName(UWorld* _World, const FString& _AssetName)
{
	UBaseGameInstance* Inst = _World->GetGameInstance<UBaseGameInstance>();
	if (!Inst || !Inst->PlayLevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("FindPlayLevelDataByAssetName: 데이터 테이블이 존재하지 않습니다."));
		return nullptr;
	}

	TArray<FName> RowNames = Inst->PlayLevelDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FPlayLevelDataRow* Row = Inst->PlayLevelDataTable->FindRow<FPlayLevelDataRow>(RowName, nullptr);
		if (Row && Row->Level.GetAssetName() == _AssetName)
		{
			UE_LOG(FALL_DEV_LOG, Log, TEXT("AssetName '%s'에 해당하는 Row를 찾았습니다. RowName: %s"), *_AssetName, *RowName.ToString());
			return Row;
		}
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("FindPlayLevelDataByAssetName: '%s' 이름을 가진 레벨을 찾을 수 없습니다."), *_AssetName);
	return nullptr;
}

// 팀플레이 레벨의 에셋 이름으로 데이터 조회
const FTeamPlayLevelDataRow* UGlobalDataTable::FindTeamPlayLevelDataByAssetName(UWorld* _World, const FString& _AssetName)
{
	UBaseGameInstance* Inst = _World->GetGameInstance<UBaseGameInstance>();
	if (!Inst || !Inst->TeamPlayLevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("FindTeamPlayLevelDataByAssetName: 데이터 테이블이 존재하지 않습니다."));
		return nullptr;
	}

	TArray<FName> RowNames = Inst->TeamPlayLevelDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FTeamPlayLevelDataRow* Row = Inst->TeamPlayLevelDataTable->FindRow<FTeamPlayLevelDataRow>(RowName, nullptr);
		if (Row && Row->Level.GetAssetName() == _AssetName)
		{
			UE_LOG(FALL_DEV_LOG, Log, TEXT("AssetName '%s'에 해당하는 Row를 찾았습니다. RowName: %s"), *_AssetName, *RowName.ToString());
			return Row;
		}
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("FindTeamPlayLevelDataByAssetName: '%s' 이름을 가진 레벨을 찾을 수 없습니다."), *_AssetName);
	return nullptr;
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
