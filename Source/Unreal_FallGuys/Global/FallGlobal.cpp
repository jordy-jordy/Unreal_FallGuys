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

void UFallGlobal::ChangeCostume(APawn* _Pawn, FString _CostumeName)
{
	UWorld* World = _Pawn->GetWorld();
	const FCostumeDataRow* CostumeData = UGlobalDataTable::GetCostumeData(World, _CostumeName);
	if (CostumeData && CostumeData->CostumeMesh)
	{
		// _Pawn의 스켈레탈 메시 가져오기
		if (USkeletalMeshComponent* MeshComp = _Pawn->FindComponentByClass<USkeletalMeshComponent>()) 
		{
			MeshComp->SetSkeletalMesh(CostumeData->CostumeMesh);
			//MeshComp->SetRelativeLocation(CostumeData->CostumePos);
			//MeshComp->SetRelativeRotation(FRotator::ZeroRotator); // 필요하면 CostumePivot 반영
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("Invalid Costume Data or Mesh"));
	}
}
