// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/FallGlobal.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include <Unreal_FallGuys.h>
#include <Global/BaseGameInstance.h>


void UFallGlobal::AssetPackagePath(UClass* _Class, const FString& _AssetName, FString& _Path)
{
	TArray<FAssetData> AllAsset = AssetsPath(_Class);

	FName CheckName = FName(*_AssetName);

	for (const FAssetData& AssetData : AllAsset)
	{
		if (AssetData.AssetName == CheckName)
		{
			_Path = AssetData.PackageName.ToString();
			return;
		}
	}
}

TArray<FAssetData> UFallGlobal::AssetsPath(UClass* _Class)
{
	TArray<FAssetData> MapList;
	if (false == FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
	{
		return MapList;
	}

	IAssetRegistry& AssetRegistry = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

	if (AssetRegistry.IsLoadingAssets())
	{
		return MapList;
	}

	if (AssetRegistry.GetAssetsByClass(_Class->GetClassPathName(), MapList))
	{
		return MapList;
	}

	return MapList;
}

// 서버 시작
void UFallGlobal::ServerStart(UObject* _Object, FString _Port)
{
	UWorld* CurWorld = _Object->GetWorld();
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Object->GetWorld()->GetGameInstance());
	GameIns->CServerStart(CurWorld, _Port);
}

// 서버 접속
void UFallGlobal::ServerConnect(UObject* _Object, FString _IP, FString _Port)
{
	UWorld* CurWorld = _Object->GetWorld();
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Object->GetWorld()->GetGameInstance());
	GameIns->CServerConnect(CurWorld, _IP, _Port);
}

// Pawn의 코스튬 변경
void UFallGlobal::ChangeCostume(APawn* _Pawn, const FString& _CostumeName)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostume(_Pawn, _CostumeName);
}

// 저장된 코스튬의 이름 반환
FString UFallGlobal::GetCostumeName(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeName();
}

// 저장된 코스튬의 스켈레탈 메시 반환
USkeletalMesh* UFallGlobal::GetCostumeMesh(APawn* _Pawn, const FString& _MeshName/* = TEXT("NULL")*/)
{
	const FCostumeDataRow* CostumeData = UGlobalDataTable::GetCostumeData(_Pawn->GetWorld(), _MeshName);
	if (CostumeData && CostumeData->CostumeMesh)
	{
		return CostumeData->CostumeMesh;
	}
	else
	{
		if (_MeshName.IsEmpty())
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("GetCostumeMesh :: Empty Costume Data or Mesh"));
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Error, TEXT("GetCostumeMesh :: Invalid Costume Data or Mesh"));
		}
	}

	return nullptr;
}

FString UFallGlobal::GetNickname(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetNickname();
}

void UFallGlobal::ChangeNickname(APawn* _Pawn, const FString& _NewNickname)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsChangeNickname(_NewNickname);
}
