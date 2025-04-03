// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include <Net/UnrealNetwork.h>
#include <Sockets.h>
#include <SocketSubsystem.h>
#include <IPAddress.h>
#include <Interfaces/IPv4/IPv4Address.h>

#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/FallConst.h>
#include <Global/Data/GlobalDataTable.h>
#include <Mode/01_Play/PlayGameMode.h>
#include <Mode/01_Play/PlayGameState.h>
#include <Mode/01_Play/PlayPlayerState.h>
#include <Mode/01_Play/PlayCharacter.h>


UBaseGameInstance::UBaseGameInstance()
{
	UE_LOG(FALL_DEV_LOG, Log, TEXT("%S(%u)> DataTableLoading Start"), __FUNCTION__, __LINE__);
	{
		FString DataPath = UFallConst::GlobalDataTablePath;
		ConstructorHelpers::FObjectFinder<UDataTable> FinderDataTables(*DataPath);
		if (true == FinderDataTables.Succeeded())
		{
			DataTables = FinderDataTables.Object;
		}

		if (nullptr != DataTables)
		{
			// 코스튬 데이터 로드
			CostumeDataTable = DataTables->FindRow<FDataTableRow>("DT_CostumeDataTable", nullptr)->Resources;
			if (nullptr == CostumeDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == CostumeDataTable)"), __FUNCTION__, __LINE__);
			}

			// 코스튬 컬러 데이터 로드
			CostumeColorDataTable = DataTables->FindRow<FDataTableRow>("DT_CostumeColorDataTable", nullptr)->Resources;
			if (nullptr == CostumeColorDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == DT_CostumeColorDataTable)"), __FUNCTION__, __LINE__);
			}

			// 플레이 레벨(맵) 데이터 로드
			PlayLevelDataTable = DataTables->FindRow<FDataTableRow>("DT_PlayLevelDataTable", nullptr)->Resources;
			if (nullptr == PlayLevelDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == PlayLevelDataTable)"), __FUNCTION__, __LINE__);
			}

			// 팀플레이 레벨(맵) 데이터 로드
			TeamPlayLevelDataTable = DataTables->FindRow<FDataTableRow>("DT_TeamPlayLevelDataTable", nullptr)->Resources;
			if (nullptr == TeamPlayLevelDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == TeamPlayLevelDataTable)"), __FUNCTION__, __LINE__);
			}

			// 리소스 데이터 로드
			ResourceDataTable = DataTables->FindRow<FDataTableRow>("DT_ResourceDataTable", nullptr)->Resources;
			if (nullptr == ResourceDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == ResourceDataTable)"), __FUNCTION__, __LINE__);
			}
		}
	}
	UE_LOG(FALL_DEV_LOG, Log, TEXT("%S(%u)> DataTableLoading End"), __FUNCTION__, __LINE__);
}

#pragma region BaseGameInstance :: 서버 관련
// 서버 오픈
void UBaseGameInstance::CServerStart(UWorld* _World, FString _Port)
{
	if (bIsConnected)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("현재 서버에 연결된 상태이므로 서버를 다시 시작할 수 없습니다."));
		return;
	}

	if (!_World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerStart: _World is nullptr"));
		return;
	}

	FString OpenLevel;
	FString LevelPath = TEXT("");

	UFallGlobal::AssetPackagePath(UWorld::StaticClass(), UFallConst::PlayLevelName, LevelPath);
	OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);

	UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
}

// 서버 오픈 : 레벨 선택 필요
void UBaseGameInstance::InsSelectedServerStart(UWorld* _World, FString _Port, FString _OpenLevel)
{
	if (bIsConnected)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("현재 서버에 연결된 상태이므로 서버를 다시 시작할 수 없습니다."));
		return;
	}

	if (!_World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerStart: _World is nullptr"));
		return;
	}

	FString OpenLevel;
	FString LevelPath = TEXT("");

	UFallGlobal::AssetPackagePath(UWorld::StaticClass(), _OpenLevel, LevelPath);
	OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);

	UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
}

// 서버 접속
void UBaseGameInstance::CServerConnect(UWorld* _World, FString _IP, FString _Port)
{
	if (bIsConnected)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("이미 서버에 연결된 상태입니다."));
		return;
	}

	// 주소 생성
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	// IP 유효성 검사
	bool ValidIP = false;
	Addr->SetIp(*_IP, ValidIP);

	// Port 유효성 검사
	int32 PortNum = FCString::Atoi(*_Port);
	Addr->SetPort(PortNum);

	if (PortNum <= 0 || PortNum > 65535)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("잘못된 포트 번호 범위: %d"), PortNum);
		return;
	}

	if (!_Port.IsNumeric())
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("포트 번호가 숫자가 아닙니다: %s"), *_Port);
		return;
	}

	if (!_World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerConnect: _World is nullptr"));
		return;
	}

	FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *_IP, *_Port);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("서버 접속 시도: %s"), *ConnectLevelName);
	UGameplayStatics::OpenLevel(_World, FName(*ConnectLevelName));
}
#pragma endregion

#pragma region BaseGameInstance :: 데이터 관련
// 플레이 레벨 데이터 테이블을 얻는 함수
UDataTable* UBaseGameInstance::GetPlayLevelDataTable() const
{
	return PlayLevelDataTable;
}

// 팀플레이 레벨 데이터 테이블을 얻는 함수
UDataTable* UBaseGameInstance::GetTeamPlayLevelDataTable() const
{
	return TeamPlayLevelDataTable;
}
#pragma endregion

#pragma region BaseGameInstance :: 코스튬 관련
// 코스튬 컬러 저장
void UBaseGameInstance::InsSaveCostumeColor(const FString& _CostumeColor)
{
	CostumeColor = _CostumeColor;
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("변경한 코스튬 색상 : %s"), *CostumeColor);
}

// 코스튬 상의 저장
void UBaseGameInstance::InsSaveCostumeTop(const FString& _CostumeTop)
{
	CostumeTop = _CostumeTop;
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("착용한 코스튬(상의) 이름 : %s"), *CostumeTop);
}

// 코스튬 하의 저장
void UBaseGameInstance::InsSaveCostumeBot(const FString& _CostumeBot)
{
	CostumeBot = _CostumeBot;
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("착용한 코스튬(하의) 이름 : %s"), *CostumeBot);
}

// Pawn의 코스튬 컬러 변경
void UBaseGameInstance::InsChangeCostumeColor(APawn* _Pawn, const FString& _CostumeColor)
{
	UWorld* World = _Pawn->GetWorld();
	const FCostumeColorDataRow* CostumeColorData = UGlobalDataTable::GetCostumeColorData(World, _CostumeColor);

	if (CostumeColorData && CostumeColorData->CostumeMesh)
	{
		if (USkeletalMeshComponent* MeshComp = _Pawn->FindComponentByClass<USkeletalMeshComponent>())
		{
			MeshComp->SetSkeletalMesh(CostumeColorData->CostumeMesh);
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostumeColor :: MeshComponent가 없음"));
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostumeColor :: 잘못된 데이터 또는 메시 없음"));
	}
	InsSaveCostumeColor(_CostumeColor);
}

// 코스튬(상, 하의) 공통 함수
void UBaseGameInstance::InsApplyStaticMesh(UStaticMeshComponent* _Comp, UStaticMesh* _Mesh, const FString& _LogContext)
{
	if (!_Comp)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("%s :: 컴포넌트가 nullptr입니다."), *_LogContext);
		return;
	}

	if (_Mesh)
	{
		_Comp->SetStaticMesh(_Mesh);
	}
	else
	{
		_Comp->SetStaticMesh(nullptr);
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("%s :: 메시가 nullptr입니다."), *_LogContext);
	}
}

// Pawn의 코스튬 상의 변경
void UBaseGameInstance::InsChangeCostumeTop(APawn* _Pawn, UStaticMeshComponent* _UpComp, const FString& _CostumeTop)
{
	if (_CostumeTop == TEXT("Default") || _CostumeTop.IsEmpty())
	{
		InsApplyStaticMesh(_UpComp, nullptr, TEXT("InsChangeCostumeTop(Default)"));
		InsSaveCostumeTop(_CostumeTop);
		return;
	}

	UWorld* World = _Pawn->GetWorld();
	const FCostumeDataRow* CostumeTopData = UGlobalDataTable::GetCostumeData(World, _CostumeTop);

	InsApplyStaticMesh(_UpComp,
		CostumeTopData ? CostumeTopData->CostumeMesh : nullptr,
		TEXT("InsChangeCostumeTop"));

	InsSaveCostumeTop(_CostumeTop);
}

// Pawn의 코스튬 하의 변경
void UBaseGameInstance::InsChangeCostumeBot(APawn* _Pawn, UStaticMeshComponent* _LowComp, const FString& _CostumeBot)
{
	if (_CostumeBot == TEXT("Default") || _CostumeBot.IsEmpty())
	{
		InsApplyStaticMesh(_LowComp, nullptr, TEXT("InsChangeCostumeBot(Default)"));
		InsSaveCostumeBot(_CostumeBot);
		return;
	}

	UWorld* World = _Pawn->GetWorld();
	const FCostumeDataRow* CostumeBotData = UGlobalDataTable::GetCostumeData(World, _CostumeBot);

	InsApplyStaticMesh(_LowComp,
		CostumeBotData ? CostumeBotData->CostumeMesh : nullptr,
		TEXT("InsChangeCostumeBot"));

	InsSaveCostumeBot(_CostumeBot);
}

// 저장된 코스튬 컬러의 스켈레탈 메시 반환
USkeletalMesh* UBaseGameInstance::InsGetCostumeColorMesh(APawn* _Pawn, const FString& _MeshName)
{
	const FCostumeColorDataRow* CostumeColorData = UGlobalDataTable::GetCostumeColorData(_Pawn->GetWorld(), _MeshName);
	if (CostumeColorData && CostumeColorData->CostumeMesh)
	{
		return CostumeColorData->CostumeMesh;
	}
	else
	{
		if (_MeshName.IsEmpty())
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetCostumeMesh :: Empty Costume Color Mesh Data or Mesh"));
		}
		// 에러 임시 비활성화
		//else
		//{
		//	UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetCostumeMesh :: Invalid Costume Color Mesh Data or Mesh"));
		//}
	}
	return nullptr;
}

// 코스튬 컬러 UI용 머티리얼 반환 함수
UMaterialInterface* UBaseGameInstance::InsGetCostumeColorUIMaterial(const FString& _ColorName)
{
	if (!CostumeColorDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetCostumeColorUIMaterial :: CostumeColorDataTable이 nullptr입니다."));
		return nullptr;
	}

	const FCostumeColorDataRow* Row = UGlobalDataTable::GetCostumeColorData(GetWorld(), _ColorName);
	if (Row && Row->ColorUIMaterial)
	{
		return Row->ColorUIMaterial;
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetCostumeColorUIMaterial :: 해당 이름(%s)의 데이터 또는 머티리얼이 없음"), *_ColorName);
	return nullptr;
}

// 저장된 코스튬의 스테틱 메시 반환
UStaticMesh* UBaseGameInstance::InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName)
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
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetCostumeMesh :: Empty Costume Mesh Data or Mesh"));
		}
		// 에러 임시 비활성화
		//else
		//{
		//	UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetCostumeMesh :: Invalid Costume Mesh Data or Mesh"));
		//}
	}
	return nullptr;
}
#pragma endregion

#pragma region BaseGameInstance :: 리소스 관련
// 리소스의 스테틱 메시 머티리얼 반환
UMaterialInterface* UBaseGameInstance::InsGetResourceMeshMaterial(const FString& _ColorName)
{
	if (!ResourceDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetResourceMeshMaterial :: ResourceDataTable이 nullptr입니다."));
		return nullptr;
	}

	const FResourceDataRow* Row = UGlobalDataTable::GetResourceData(GetWorld(), _ColorName);
	if (Row && Row->Material)
	{
		return Row->Material;
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetResourceMeshMaterial :: 해당 이름(%s)의 데이터 또는 머티리얼이 없음"), *_ColorName);
	return nullptr;
}

// 리소스의 스테틱 메시 반환
UStaticMesh* UBaseGameInstance::InsGetResourceMesh(UWorld* _World, const FString& _MeshName)
{
	const FResourceDataRow* ResourceData = UGlobalDataTable::GetResourceData(_World, _MeshName);
	if (ResourceData && ResourceData->Mesh)
	{
		return ResourceData->Mesh;
	}
	else
	{
		if (_MeshName.IsEmpty())
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetResourceMesh :: Empty Resource Data or Mesh"));
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetResourceMesh :: Invalid Resource Data or Mesh"));
		}
	}

	return nullptr;
}
#pragma endregion

#pragma region BaseGameInstance :: 레벨 관련
// Random PlayLevel의 이름 반환
FString UBaseGameInstance::InsGetRandomLevel()
{
	if (MapList.Num() == 0 || LevelNameMap.Num() == 0)
	{
		TArray<FLevelDisplayInfo> LevelInfos = UFallGlobal::GetAvailableLevelInfos();

		for (const FLevelDisplayInfo& Info : LevelInfos)
		{
			MapList.Add(Info.AssetName);               // 기존 MapList에는 AssetName 저장
			LevelNameMap.Add(Info.AssetName, Info.Name); // AssetName → Name 매핑
		}
	}

	if (MapList.Num() == 0)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetRandomLevel :: 사용 가능한 맵이 없습니다."));
		return TEXT("DefaultMap");
	}

	int RandomIndex = 0;
	do {
		RandomIndex = FMath::RandRange(0, MapList.Num() - 1);
	} while (PlayedMapList.Contains(RandomIndex));

	PlayedMapList.Add(RandomIndex);

	const FString& SelectedAssetName = MapList[RandomIndex];
	CurLevelName = LevelNameMap.Contains(SelectedAssetName) ? LevelNameMap[SelectedAssetName] : TEXT("Unknown");
	CurLevelAssetName = SelectedAssetName;

	return SelectedAssetName;
}

// Random TeamPlayLevel의 이름 반환
FString UBaseGameInstance::InsGetRandomTeamLevel()
{
	if (MapList.Num() == 0 || LevelNameMap.Num() == 0)
	{
		TArray<FLevelDisplayInfo> LevelInfos = UFallGlobal::GetAvailableTeamPlayLevelInfos();

		for (const FLevelDisplayInfo& Info : LevelInfos)
		{
			MapList.Add(Info.AssetName);               // 기존 MapList에는 AssetName 저장
			LevelNameMap.Add(Info.AssetName, Info.Name); // AssetName → Name 매핑
		}
	}

	if (MapList.Num() == 0)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetRandomTeamLevel :: 사용 가능한 맵이 없습니다."));
		return TEXT("DefaultMap");
	}

	int RandomIndex = 0;
	do {
		RandomIndex = FMath::RandRange(0, MapList.Num() - 1);
	} while (PlayedMapList.Contains(RandomIndex));

	PlayedMapList.Add(RandomIndex);

	const FString& SelectedAssetName = MapList[RandomIndex];
	CurLevelName = LevelNameMap.Contains(SelectedAssetName) ? LevelNameMap[SelectedAssetName] : TEXT("Unknown");
	CurLevelAssetName = SelectedAssetName;

	return SelectedAssetName;
}

// 레벨 가이드 반환
FString UBaseGameInstance::InsGetPlayGuideFromAssetName(const FString& _AssetName)
{
	if (!PlayLevelDataTable && !TeamPlayLevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetPlayGuideFromAssetName :: 데이터 테이블이 모두 nullptr입니다."));
		return TEXT("Unknown");
	}

	const FPlayLevelDataRow* Row = nullptr;
	const FTeamPlayLevelDataRow* TeamRow = nullptr;

	if (PlayLevelDataTable)
	{
		Row = UFallGlobal::FindRowByFStringField<FPlayLevelDataRow>(
			PlayLevelDataTable,
			_AssetName,
			TEXT("InsGetPlayGuideFromAssetName"),
			[](const FPlayLevelDataRow* R) { return R->Level.GetAssetName(); }
		);
	}

	if (TeamPlayLevelDataTable)
	{
		TeamRow = UFallGlobal::FindRowByFStringField<FTeamPlayLevelDataRow>(
			TeamPlayLevelDataTable,
			_AssetName,
			TEXT("InsGetPlayGuideFromAssetName_Team"),
			[](const FTeamPlayLevelDataRow* R) { return R->Level.GetAssetName(); }
		);
	}

	if (Row && TeamRow)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetPlayGuideFromAssetName :: AssetName(%s)가 두 개의 테이블에 모두 존재합니다."), *_AssetName);
	}

	if (Row) { return Row->PlayGuide; }
	if (TeamRow) { return TeamRow->PlayGuide; }

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetPlayGuideFromAssetName :: AssetName(%s)의 가이드를 찾을 수 없습니다."), *_AssetName);
	return TEXT("Unknown");
}

// 레벨 이미지 반환
UTexture2D* UBaseGameInstance::InsGetLevelImageFromAssetName(const FString& _AssetName)
{
	if (!PlayLevelDataTable && !TeamPlayLevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetLevelImageFromAssetName :: 데이터 테이블이 모두 nullptr입니다."));
		return nullptr;
	}

	const FPlayLevelDataRow* Row = nullptr;
	const FTeamPlayLevelDataRow* TeamRow = nullptr;

	if (PlayLevelDataTable)
	{
		Row = UFallGlobal::FindRowByFStringField<FPlayLevelDataRow>(
			PlayLevelDataTable,
			_AssetName,
			TEXT("InsGetLevelImageFromAssetName"),
			[](const FPlayLevelDataRow* R) { return R->Level.GetAssetName(); }
		);
	}

	if (TeamPlayLevelDataTable)
	{
		TeamRow = UFallGlobal::FindRowByFStringField<FTeamPlayLevelDataRow>(
			TeamPlayLevelDataTable,
			_AssetName,
			TEXT("InsGetLevelImageFromAssetName_Team"),
			[](const FTeamPlayLevelDataRow* R) { return R->Level.GetAssetName(); }
		);
	}

	if (Row && TeamRow)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetLevelImageFromAssetName :: AssetName(%s)가 두 개의 테이블에 모두 존재합니다."), *_AssetName);
	}

	if (Row && Row->LevelIMG) { return Row->LevelIMG; }
	if (TeamRow && TeamRow->LevelIMG) { return TeamRow->LevelIMG; }

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetLevelImageFromAssetName :: AssetName(%s)의 이미지를 찾을 수 없습니다."), *_AssetName);
	return nullptr;
}

// 플레이 목표 반환
FString UBaseGameInstance::InsGetGoalGuideFromAssetName(const FString& _AssetName)
{
	if (!PlayLevelDataTable && !TeamPlayLevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetGoalGuideFromAssetName :: 데이터 테이블이 모두 nullptr입니다."));
		return TEXT("Unknown");
	}

	const FPlayLevelDataRow* Row = nullptr;
	const FTeamPlayLevelDataRow* TeamRow = nullptr;

	// PlayLevelDataTable에서 검색
	if (PlayLevelDataTable)
	{
		Row = UFallGlobal::FindRowByFStringField<FPlayLevelDataRow>(
			PlayLevelDataTable,
			_AssetName,
			TEXT("InsGetGoalGuideFromAssetName"),
			[](const FPlayLevelDataRow* R) { return R->Level.GetAssetName(); }
		);
	}

	// TeamPlayLevelDataTable에서 검색
	if (TeamPlayLevelDataTable)
	{
		TeamRow = UFallGlobal::FindRowByFStringField<FTeamPlayLevelDataRow>(
			TeamPlayLevelDataTable,
			_AssetName,
			TEXT("InsGetGoalGuideFromAssetName_Team"),
			[](const FTeamPlayLevelDataRow* R) { return R->Level.GetAssetName(); }
		);
	}

	// 둘 다 있는 경우 경고 로그 출력
	if (Row && TeamRow)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetGoalGuideFromAssetName :: AssetName(%s)가 두 개의 테이블에 모두 존재합니다."), *_AssetName);
	}

	if (Row) { return Row->GoalGuide; }
	if (TeamRow) { return TeamRow->GoalGuide; }

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetGoalGuideFromAssetName :: AssetName(%s)의 GoalGuide를 찾을 수 없습니다."), *_AssetName);
	return TEXT("Unknown");
}
#pragma endregion

#pragma region BaseGameInstance :: 플레이어 데이터 관련
// 플레이어 정보 백업 함수
void UBaseGameInstance::InsBackupPlayerInfo(const FString& _UniqueID, const FPlayerInfo& _PlayerInfo)
{
	if (_UniqueID.IsEmpty())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsBackupPlayerInfo :: Invalid UniqueID"));
		return;
	}

	PlayerInfoBackup.Add(_UniqueID, _PlayerInfo);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("InsBackupPlayerInfo :: UniqueID = %s, PlayerTag = %s"),
		*_UniqueID, *_PlayerInfo.Tag);
}

// 백업된 플레이어 정보 복구 함수
bool UBaseGameInstance::InsGetBackedUpPlayerInfo(const FString& _UniqueID, FPlayerInfo& _OutPlayerInfo) const
{
	const FPlayerInfo* FoundInfo = PlayerInfoBackup.Find(_UniqueID);
	if (FoundInfo)
	{
		_OutPlayerInfo = *FoundInfo;
		return true;
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetBackedUpPlayerInfo :: No PlayerInfo found for UniqueID = %s"),
		*_UniqueID);
	return false;
}

// 닉네임 변경
void UBaseGameInstance::InsChangeNickname(const FString& _NewNickname)
{
	Nickname = _NewNickname;

	// 닉네임 설정한 상태로 전환
	HasNickname = true;
}
#pragma endregion

#pragma region BaseGameInstance :: 개발용
// 디버그용 : 플레이어 태그 확인
void UBaseGameInstance::InsPrintPlayerInfo()
{
	APlayGameState* PlayGameState = GetWorld()->GetGameState<APlayGameState>();
	if (!PlayGameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PrintPlayerInfo: GameState가 nullptr 입니다."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: GameState가 nullptr 입니다."));
		}
		return;
	}

	// 콘솔 로그
	UE_LOG(FALL_DEV_LOG, Log, TEXT("=== PlayerInfoArray 목록 ==="));

	// 화면 출력용 문자열
	FString ScreenMessage = TEXT("=== PlayerInfoArray 목록 ===\n");

	for (const FPlayerInfoEntry& Entry : PlayGameState->PlayerInfoArray)
	{
		FString StatusStr = UEnum::GetValueAsString(Entry.PlayerInfo.Status);
		FString LogMessage = FString::Printf(TEXT("UniqueID: %s, Tag: %s, Status: %s"),
			*Entry.UniqueID, *Entry.PlayerInfo.Tag, *StatusStr);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("%s"), *LogMessage);

		// 화면에 출력 (GEngine 사용)
		if (GEngine)
		{
			ScreenMessage += LogMessage + TEXT("\n");
		}
	}

	UE_LOG(FALL_DEV_LOG, Log, TEXT("=== 각 플레이어의 PlayerInfo ==="));
	ScreenMessage += TEXT("=== 각 플레이어의 PlayerInfo ===\n");

	for (APlayerState* PlayerState : PlayGameState->PlayerArray)
	{
		APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayerState);
		if (PlayPlayerState)
		{
			FString StatusStr = UEnum::GetValueAsString(PlayPlayerState->PlayerInfo.Status);
			FString LogMessage = FString::Printf(TEXT("UniqueID: %s, Tag: %s, Status: %s"),
				*PlayPlayerState->PlayerInfo.UniqueID, *PlayPlayerState->PlayerInfo.Tag, *StatusStr);

			UE_LOG(FALL_DEV_LOG, Log, TEXT("%s"), *LogMessage);

			if (GEngine)
			{
				ScreenMessage += LogMessage + TEXT("\n");
			}
		}
	}

	// 최종적으로 화면에 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, ScreenMessage);
	}
}

// 디버그용 : 현재 접속자 수, 카운트 다운 END 여부, 현재 스테이지 단계, 목표 골인 인원 수 확인
void UBaseGameInstance::InsPrintConnectedPlayers()
{
	APlayGameState* PlayGameState = GetWorld()->GetGameState<APlayGameState>();
	if (!PlayGameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsPrintConnectedPlayers: GameState가 nullptr 입니다."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: GameState가 nullptr 입니다."));
		}
		return;
	}

	int32 ConnectedCount = PlayGameState->GetConnectedPlayers();
	bool IsOverCount = PlayGameState->GetIsCountDownOver();
	EStageType Stage = PlayGameState->CurrentStage;

	// FinishPlayer 값 가져오기
	APlayGameMode* GameMode = GetWorld()->GetAuthGameMode<APlayGameMode>();
	int32 FinishPlayerCount = (GameMode) ? GameMode->GetFinishPlayerCount() : -1;

	// 로그 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 접속자 수 : %d"), ConnectedCount);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("카운트 다운 끝났니? : %s"), IsOverCount ? TEXT("true") : TEXT("false"));
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 스테이지 : %s"), *UEnum::GetValueAsString(Stage));
	UE_LOG(FALL_DEV_LOG, Log, TEXT("목표 골인 인원 수 : %d"), FinishPlayerCount);

	// 화면 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("현재 접속자 수 : %d"), ConnectedCount));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("카운트 다운 끝났니? : %s"), IsOverCount ? TEXT("true") : TEXT("false")));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("현재 스테이지 : %s"), *UEnum::GetValueAsString(Stage)));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("목표 골인 인원 수 : %d"), FinishPlayerCount));
	}
}

// 디버그용 : 레벨 이름 확인
void UBaseGameInstance::InsPrintLevelName()
{
	APlayGameState* PlayGameState = GetWorld()->GetGameState<APlayGameState>();
	if (!PlayGameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsPrintLevelName: GameState가 nullptr 입니다."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: GameState가 nullptr 입니다."));
		}
		return;
	}

	FString LevelName = PlayGameState->GetLevelName();

	// 콘솔 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 PlayGameState의 LevelName: %s"), *LevelName);

	// 화면 출력
	if (GEngine)
	{
		FString ScreenMessage = FString::Printf(TEXT("현재 레벨 이름: %s"), *LevelName);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, ScreenMessage);
	}
}

// 디버그용 : 레벨 시네마틱 바로 끝나게
void UBaseGameInstance::InsetLevelCinematicEnd()
{
	APlayGameState* PlayGameState = GetWorld()->GetGameState<APlayGameState>();
	if (!PlayGameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsPrintLevelName: GameState가 nullptr 입니다."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: GameState가 nullptr 입니다."));
		}
		return;
	}

	// TRUE로 바뀌기 전
	bool ValueBefore = PlayGameState->GetIsLevelCinematicEnd();

	// 콘솔 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 레벨 시네마틱 상태 : %s"), ValueBefore ? TEXT("true") : TEXT("false"));

	// 화면 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("현재 레벨 시네마틱 상태 : %s"), ValueBefore ? TEXT("true") : TEXT("false")));
	}

	// TRUE로 바꿈
	PlayGameState->SetIsLevelCinematicEnd(true);

	bool ValueAfter = PlayGameState->GetIsLevelCinematicEnd();

	// 콘솔 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("레벨 시네마틱 끝났니? : %s"), ValueAfter ? TEXT("true") : TEXT("false"));

	// 화면 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("레벨 시네마틱 끝났니? : %s"), ValueAfter ? TEXT("true") : TEXT("false")));
	}
}

// 디버그용 : 골인 목표 인원, 골인한 인원 출력
void UBaseGameInstance::InsGetGameStateCurFinishPlayer()
{
	APlayGameState* PlayGameState = GetWorld()->GetGameState<APlayGameState>();
	if (!PlayGameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsPrintLevelName: GameState가 nullptr 입니다."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: GameState가 nullptr 입니다."));
		}
		return;
	}

	int CurGoalCount = PlayGameState->GetGameStateCurFinishPlayer();
	int TargetGoalCount = PlayGameState->GetGameStateFinishPlayer();

	// 콘솔 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 골인한 인원 : %d"), CurGoalCount);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("목표 골인 인원 : %d"), TargetGoalCount);

	// 화면 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("현재 골인한 인원 : %d"), CurGoalCount));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("목표 골인 인원 : %d"), TargetGoalCount));
	}
}

// 디버그용 : 플레이어 성공시키고 상태 출력
void UBaseGameInstance::InsSetPlayerDie(APlayerController* _Controller)
{
	APlayCharacter* PlayerCH = Cast<APlayCharacter>(_Controller->GetCharacter());
	APlayPlayerState* PlayerState = PlayerCH->GetPlayerState<APlayPlayerState>();

	PlayerCH->C2S_IsDie(false);
	FString PlayerStat = UEnum::GetValueAsString(PlayerState->PlayerInfo.Status);
	
	bool Value = PlayerCH->IsDie;

	UE_LOG(FALL_DEV_LOG, Log, TEXT("플레이어 상태(bool) : %s"), Value ? TEXT("True") : TEXT("False"));
	UE_LOG(FALL_DEV_LOG, Log, TEXT("플레이어 상태(enum) : %s"), *PlayerStat);
}

#pragma endregion
