// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/FallGlobal.h"
#include "AssetRegistry/AssetRegistryModule.h"

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

void UFallGlobal::ServerStart(UObject* _Object, FString _Port)
{
	UWorld* CurWorld = _Object->GetWorld();
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Object->GetWorld()->GetGameInstance());
	GameIns->CServerStart(CurWorld, _Port);
}

void UFallGlobal::ServerConnect(UObject* _Object, FString _IP, FString _Port)
{
	UWorld* CurWorld = _Object->GetWorld();
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Object->GetWorld()->GetGameInstance());
	GameIns->CServerConnect(CurWorld, _IP, _Port);
}

