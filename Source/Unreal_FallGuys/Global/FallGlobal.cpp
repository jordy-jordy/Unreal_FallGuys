// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/FallGlobal.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include <Engine/DataTable.h>
#include <Unreal_FallGuys.h>
#include <Global/BaseGameInstance.h>
#include <Global/Data/PlayLevelDataTable.h>


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

// 서버 시작 : 선택 필요
void UFallGlobal::SelectedServerStart(UObject* _Object, FString _Port, FString _OpenLevel)
{
	UWorld* CurWorld = _Object->GetWorld();
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Object->GetWorld()->GetGameInstance());
	GameIns->InsSelectedServerStart(CurWorld, _Port, _OpenLevel);
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

// 닉네임 반환
FString UFallGlobal::GetNickname(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetNickname();
}

// 닉네임 저장
void UFallGlobal::ChangeNickname(APawn* _Pawn, const FString& _NewNickname)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsChangeNickname(_NewNickname);
}

// 플레이 가능한 레벨 반환
TArray<FString> UFallGlobal::GetAvailableLevels()
{
	TArray<FString> LevelNames;

	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GWorld->GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("GetAvailableLevels: GameInstance is null!"));
		return LevelNames;
	}

	// PlayLevelDataTable 가져오기
	UDataTable* LevelDataTable = GameInstance->GetPlayLevelDataTable(); // 이 함수는 BaseGameInstance에서 PlayLevelDataTable을 가져오는 함수로 가정
	if (!LevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("GetAvailableLevels: PlayLevelDataTable is null!"));
		return LevelNames;
	}

	// 데이터 테이블의 모든 행 가져오기
	static const FString ContextString(TEXT("Name"));
	TArray<FPlayLevelDataRow*> LevelRows;
	LevelDataTable->GetAllRows<FPlayLevelDataRow>(ContextString, LevelRows);

	for (const FPlayLevelDataRow* Row : LevelRows)
	{
		if (Row)
		{
			UE_LOG(FALL_DEV_LOG, Log, TEXT("Row Name: %s"), *Row->Name);
			UE_LOG(FALL_DEV_LOG, Log, TEXT("Row Level Path: %s"), *Row->Level.ToString());

			// 강제로 동기 로드하여 확인
			UWorld* LoadedLevel = Row->Level.LoadSynchronous();
			if (!LoadedLevel)
			{
				UE_LOG(FALL_DEV_LOG, Warning, TEXT("Row->Level failed to load synchronously!"));
			}
			else
			{
				UE_LOG(FALL_DEV_LOG, Log, TEXT("Row->Level loaded successfully!"));
			}
		}

		if (Row && Row->Level.IsValid())
		{
			FString LevelName = Row->Level.GetAssetName();
			LevelNames.Add(LevelName);
		}
	}

	return LevelNames;
}

// 랜덤 스테이지 반환
FString UFallGlobal::GetRandomLevel(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetRandomLevel();
}
