// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "AssetRegistry/AssetRegistryModule.h"


void UTitleWidget::AssetPackagePath(UClass* _Class, const FString& _AssetName, FString& _Path)
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

TArray<FAssetData> UTitleWidget::AssetsPath(UClass* _Class)
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

void UTitleWidget::StartServer()
{
	FString OpenLevel;
	FString LevelPath = TEXT("");

	UTitleWidget::AssetPackagePath(UWorld::StaticClass(), TEXT("Test_PlayLevel"), LevelPath);
	OpenLevel = FString::Printf(TEXT(":%s%s"), *Port, *LevelPath);

	UGameplayStatics::OpenLevel(GetWorld(), *OpenLevel, true, TEXT("listen"));
}

void UTitleWidget::Connect()
{
	FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *IP, *Port);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*ConnectLevelName));
}

