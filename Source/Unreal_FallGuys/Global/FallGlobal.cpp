// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/FallGlobal.h"
#include "Kismet/GameplayStatics.h"
#include "AssetRegistry/AssetRegistryModule.h"


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

void UFallGlobal::StartServer(UWorld* _World, FString _Port)
{
	FString OpenLevel;
	FString LevelPath = TEXT("");

	UFallGlobal::AssetPackagePath(UWorld::StaticClass(), TEXT("Test_PlayLevel"), LevelPath);
	OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);

	UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
}

void UFallGlobal::Connect(UWorld* _World, FString _IP, FString _Port)
{
	FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *_IP, *_Port);

	UGameplayStatics::OpenLevel(_World, FName(*ConnectLevelName));
}

