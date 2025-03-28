// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/FallGlobal.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

#include <Unreal_FallGuys.h>
#include <Global/BaseGameInstance.h>
#include <Global/Data/PlayLevelDataTable.h>
#include <Global/Data/CostumeColorDataTable.h>
#include <Mode/00_Title/TitleHUD.h>
#include <Mode/01_Play/PlayGameMode.h>
#include <Mode/01_Play/PlayGameState.h>


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

// 저장된 코스튬의 컬러 반환
FString UFallGlobal::GetCostumeColor(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeColor();
}

// 저장된 코스튬의 상의 반환
FString UFallGlobal::GetCostumeTop(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeTop();
}

// 저장된 코스튬의 하의 반환
FString UFallGlobal::GetCostumeBot(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeBot();
}

// Pawn의 코스튬 컬러 변경
void UFallGlobal::ChangeCostumeColor(APawn* _Pawn, const FString& _CostumeColor)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostumeColor(_Pawn, _CostumeColor);
}

// Pawn의 코스튬 상의 변경
void UFallGlobal::ChangeCostumeTop(APawn* _Pawn, UStaticMeshComponent* _UpComp, const FString& _CostumeTop)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostumeTop(_Pawn, _UpComp, _CostumeTop);
}

// Pawn의 코스튬 하의 변경
void UFallGlobal::ChangeCostumeBot(APawn* _Pawn, UStaticMeshComponent* _LowComp, const FString& _CostumeBot)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostumeBot(_Pawn, _LowComp, _CostumeBot);
}

// 저장된 코스튬 컬러의 스켈레탈 메시 반환
USkeletalMesh* UFallGlobal::GetCostumeColorMesh(APawn* _Pawn, const FString& _MeshName)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeColorMesh(_Pawn, _MeshName);
}

// 코스튬 컬러 UI용 머티리얼 반환
UMaterialInterface* UFallGlobal::GetCostumeColorUIMaterial(APawn* _Pawn, const FString& _ColorName)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeColorUIMaterial(_ColorName);
}

// 저장된 코스튬의 스테틱 메시 반환
UStaticMesh* UFallGlobal::GetCostumeMesh(APawn* _Pawn, const FString& _MeshName)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeMesh(_Pawn, _MeshName);
}

// 리소스의 스테틱 메시 반환
UStaticMesh* UFallGlobal::GetResourceMesh(const FString& _MeshName)
{
	UWorld* World = GWorld;
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(World->GetGameInstance());
	return GameIns->InsGetResourceMesh(World, _MeshName);
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
	TArray<FString> LevelAssetNames;

	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GWorld->GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("GetAvailableLevels: GameInstance is null!"));
		return LevelAssetNames;
	}

	// PlayLevelDataTable 가져오기
	UDataTable* LevelDataTable = GameInstance->GetPlayLevelDataTable();
	if (!LevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("GetAvailableLevels: PlayLevelDataTable is null!"));
		return LevelAssetNames;
	}

	// 데이터 테이블의 모든 행 가져오기
	static const FString ContextString(TEXT("GetAvailableLevels :: "));
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
			FString LevelAssetName = Row->Level.GetAssetName();
			LevelAssetNames.Add(LevelAssetName);
		}
	}

	return LevelAssetNames;
}

// 플레이 가능한 레벨 및 이름 반환
TArray<FLevelDisplayInfo> UFallGlobal::GetAvailableLevelInfos()
{
	TArray<FLevelDisplayInfo> LevelInfos;

	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GWorld->GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("GetAvailableLevelInfos: GameInstance is null!"));
		return LevelInfos;
	}

	UDataTable* LevelDataTable = GameInstance->GetPlayLevelDataTable();
	if (!LevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("GetAvailableLevelInfos: PlayLevelDataTable is null!"));
		return LevelInfos;
	}

	static const FString ContextString(TEXT("GetAvailableLevelInfos"));
	TArray<FPlayLevelDataRow*> LevelRows;
	LevelDataTable->GetAllRows<FPlayLevelDataRow>(ContextString, LevelRows);

	for (const FPlayLevelDataRow* Row : LevelRows)
	{
		if (Row == nullptr)
			continue;

		// 강제로 로드
		UWorld* LoadedLevel = Row->Level.LoadSynchronous();
		if (!LoadedLevel)
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("Level Load Failed: %s"), *Row->Level.ToString());
			continue;
		}

		FLevelDisplayInfo Info;
		Info.Name = Row->Name;
		Info.AssetName = Row->Level.GetAssetName();

		UE_LOG(FALL_DEV_LOG, Log, TEXT("Level Info Added - Name: %s, Asset: %s"), *Info.Name, *Info.AssetName);

		LevelInfos.Add(Info);
	}

	return LevelInfos;
}

// 랜덤 스테이지 반환
FString UFallGlobal::GetRandomLevel(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetRandomLevel();
}

// 레벨 가이드 반환
FString UFallGlobal::GetPlayGuideFromAssetName(const FString& _AssetName)
{
	UWorld* World = GWorld;
	if (!World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetPlayGuideFromAssetName: World is nullptr"));
	}

	UBaseGameInstance* GameIns = World->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetPlayGuideFromAssetName(_AssetName);
}

// 레벨 이미지 반환
UTexture2D* UFallGlobal::GetLevelImageFromAssetName(const FString& _AssetName)
{
	UWorld* World = GWorld;
	if (!World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetLevelImage: World is nullptr"));
	}

	UBaseGameInstance* GameIns = World->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetLevelImageFromAssetName(_AssetName);
}

// PlayGameState : 랜덤 레벨 함수에서 얻은 이름 반환
FString UFallGlobal::GetLevelName()
{
	UWorld* World = GWorld;
	if (!World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetConnectedPlayers: World is nullptr"));
		return TEXT("GetLevelName False");
	}

	APlayGameState* FallState = Cast<APlayGameState>(World->GetGameState());
	return FallState->GetLevelName();
}

// PlayGameState : 랜덤 레벨 함수에서 얻은 에셋 이름 반환
FString UFallGlobal::GetLevelAssetName()
{
	UWorld* World = GWorld;
	if (!World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetConnectedPlayers: World is nullptr"));
		return TEXT("GetLevelName False");
	}

	APlayGameState* FallState = Cast<APlayGameState>(World->GetGameState());
	return FallState->GetLevelAssetName();
}

// PlayGameState : 현재 접속한 플레이어 수 반환
int UFallGlobal::GetConnectedPlayers()
{
	UWorld* World = GWorld; 
	if (!World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetConnectedPlayers: World is nullptr"));
		return 0;
	}

	APlayGameState* PlayState = Cast<APlayGameState>(World->GetGameState());
	if (!PlayState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetConnectedPlayers: PlayGameState is nullptr"));
		return 0;
	}

	return PlayState->GetConnectedPlayers();
}

// PlayGameState : Count Down Time 반환 - 실시간으로 줄어드는 시간
float UFallGlobal::GetCountDownTime()
{
	UWorld* World = GWorld;
	if (!World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetCountDownTime: World is nullptr"));
		return 0;
	}

	APlayGameState* PlayState = Cast<APlayGameState>(World->GetGameState());
	if (!PlayState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetCountDownTime: PlayGameState is nullptr"));
		return 0;
	}

	return PlayState->CountDownTime;
}

// PlayGameState : Is Count Down Over 반환 - 카운트 다운 끝났어?
bool UFallGlobal::GetIsCountDownOver()
{
	UWorld* World = GWorld;
	if (!World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetCountDownTime: World is nullptr"));
		return 0;
	}

	APlayGameState* PlayState = Cast<APlayGameState>(World->GetGameState());
	if (!PlayState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetCountDownTime: PlayGameState is nullptr"));
		return 0;
	}

	return PlayState->IsCountDownOver;
}

TArray<FString> UFallGlobal::GetCostumeColorNames(UObject* _WorldContext)
{
	TArray<FString> ColorNames;

	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_WorldContext->GetWorld()->GetGameInstance());
	if (nullptr == GameIns || nullptr == GameIns->CostumeColorDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetCostumeColorNames: GameInstance나 CostumeColorDataTable이 null"));
		return ColorNames;
	}

	// 데이터 테이블의 모든 행 가져오기
	static const FString ContextString(TEXT("GetCostumeColorNames"));
	TArray<FCostumeColorDataRow*> AllRows;
	GameIns->CostumeColorDataTable->GetAllRows<FCostumeColorDataRow>(ContextString, AllRows);

	for (const FCostumeColorDataRow* Row : AllRows)
	{
		if (Row)
		{
			ColorNames.Add(Row->Name); // Name은 FString이어야 해
		}
	}

	return ColorNames;
}

// 이재영 : 메인위젯을 얻는 함수
UTitleMainWidget* UFallGlobal::GetMainWidget(UWorld* _World)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(_World, 0);

	if (nullptr == PlayerController)
	{
		UE_LOG(FALL_DEV_LOG, Fatal, TEXT("%S(%u)> if (nullptr == PlayerController)"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	ATitleHUD* TitleHUD = Cast<ATitleHUD>(PlayerController->GetHUD());

	if (nullptr == TitleHUD)
	{
		UE_LOG(FALL_DEV_LOG, Fatal, TEXT("%S(%u)> if (nullptr == PlayerController)"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return TitleHUD->GetMainWidget();
}
