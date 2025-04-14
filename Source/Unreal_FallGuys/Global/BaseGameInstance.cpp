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
#include <Global/Data/PlayLevelDataTable.h>
#include <Global/Data/TeamPlayLevelDataTable.h>
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

// 플레이 가능한 개인전 레벨 및 레벨 데이터들의 내용을 세팅
void UBaseGameInstance::InsSaveAvailableLevelInfos()
{
	if (!PlayLevelInfos.IsEmpty())
	{
		// 데이터 있으면 초기화 해줌
		PlayLevelInfos.Empty();
	}

	if (!PlayLevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("BaseGameInstance :: InsSaveAvailableLevelInfos :: PlayLevelDataTable is null!"));
		return;
	}

	static const FString ContextString(TEXT("InsSaveAvailableLevelInfos"));
	TArray<FPlayLevelDataRow*> LevelRows;
	PlayLevelDataTable->GetAllRows<FPlayLevelDataRow>(ContextString, LevelRows);

	for (const FPlayLevelDataRow* Row : LevelRows)
	{
		if (Row == nullptr || Row->UseLevel == false)
			continue;

		FLevelInfo Info;

		// 개인전용 정보
		Info.EndCondition = Row->EndCondition;

		// 공통 정보
		Info.LevelAssetName = Row->Level.GetAssetName();
		Info.LevelName = Row->Name;
		Info.LevelType = Row->LevelType;
		Info.PlayGuide = Row->PlayGuide;
		Info.GoalGuide = Row->GoalGuide;
		Info.LevelIMG = Row->LevelIMG;
		Info.LevelTagIMG = Row->LevelTagIMG;

		UE_LOG(FALL_DEV_LOG, Log, TEXT("BaseGameInstance :: 개인전 레벨이 추가 되었습니다. - 레벨 에셋 이름 : %s, 레벨 이름 : %s"), *Info.LevelAssetName, *Info.LevelName);

		PlayLevelInfos.Add(Info);
	}
}

// 플레이 가능한 팀전 레벨 및 레벨 데이터들의 내용을 세팅
void UBaseGameInstance::InsSaveAvailableTeamLevelInfos()
{
	if (!TeamPlayLevelInfos.IsEmpty())
	{
		// 데이터 있으면 초기화 해줌
		TeamPlayLevelInfos.Empty();
	}

	if (!TeamPlayLevelDataTable)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("BaseGameInstance :: InsSaveAvailableTeamLevelInfos :: TeamPlayLevelDataTable is null!"));
		return;
	}

	static const FString ContextString(TEXT("InsSaveAvailableTeamLevelInfos"));
	TArray<FTeamPlayLevelDataRow*> LevelRows;
	TeamPlayLevelDataTable->GetAllRows<FTeamPlayLevelDataRow>(ContextString, LevelRows);

	for (const FTeamPlayLevelDataRow* Row : LevelRows)
	{
		if (Row == nullptr || Row->UseLevel == false)
			continue;

		FTeamLevelInfo TeamInfo;

		// 팀전용 정보
		TeamInfo.StageLimitTime = Row->StageLimitTime;

		// 공통 정보
		TeamInfo.LevelAssetName = Row->Level.GetAssetName();
		TeamInfo.LevelName = Row->Name;
		TeamInfo.LevelType = Row->LevelType;
		TeamInfo.PlayGuide = Row->PlayGuide;
		TeamInfo.GoalGuide = Row->GoalGuide;
		TeamInfo.LevelIMG = Row->LevelIMG;
		TeamInfo.LevelTagIMG = Row->LevelTagIMG;

		UE_LOG(FALL_DEV_LOG, Log, TEXT("BaseGameInstance :: 팀전 레벨이 추가 되었습니다. - 레벨 에셋 이름 : %s, 레벨 이름 : %s"), *TeamInfo.LevelAssetName, *TeamInfo.LevelName);

		TeamPlayLevelInfos.Add(TeamInfo);
	}
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
// 랜덤 개인전 레벨 반환 : 에셋 이름 반환
FString UBaseGameInstance::InsGetRandomLevel()
{
	// 사용 가능한 레벨 정보가 없다면 저장
	if (PlayLevelInfos.IsEmpty())
	{
		InsSaveAvailableLevelInfos();
	}

	// 모든 맵을 한 번 돌았으면 초기화
	if (PlayedMapList.Num() >= PlayLevelInfos.Num())
	{
		PlayedMapList.Empty();
		UE_LOG(FALL_DEV_LOG, Log, TEXT("BaseGameInstance :: InsGetRandomLevel :: 모든 개인전 맵을 돌았으므로 초기화합니다."));
	}

	// 중복되지 않은 맵 뽑기
	FLevelInfo SelectedInfo;
	do
	{
		int32 RandomIndex = FMath::RandRange(0, PlayLevelInfos.Num() - 1);
		SelectedInfo = PlayLevelInfos[RandomIndex];
	} while (PlayedMapList.Contains(SelectedInfo.LevelAssetName));

	// 선택된 맵 등록
	PlayedMapList.Add(SelectedInfo.LevelAssetName);

	// 개인전용 정보
	CurLevelInfo_Ins.EndCondition = SelectedInfo.EndCondition;

	// 공통 정보
	CurLevelInfo_Ins.LevelAssetName = SelectedInfo.LevelAssetName;
	CurLevelInfo_Ins.LevelName = SelectedInfo.LevelName;
	CurLevelInfo_Ins.LevelType = SelectedInfo.LevelType;
	CurLevelInfo_Ins.PlayGuide = SelectedInfo.PlayGuide;
	CurLevelInfo_Ins.GoalGuide = SelectedInfo.GoalGuide;
	CurLevelInfo_Ins.LevelIMG = SelectedInfo.LevelIMG;
	CurLevelInfo_Ins.LevelTagIMG = SelectedInfo.LevelTagIMG;

	return SelectedInfo.LevelAssetName;
}

// 랜덤 팀전 레벨 반환 : 에셋 이름 반환
FString UBaseGameInstance::InsGetRandomTeamLevel()
{
	// 사용 가능한 팀전 레벨 정보가 없다면 저장
	if (TeamPlayLevelInfos.IsEmpty())
	{
		InsSaveAvailableTeamLevelInfos();
	}

	// 모든 맵을 한 번 돌았으면 초기화
	if (PlayedMapList.Num() >= TeamPlayLevelInfos.Num())
	{
		PlayedMapList.Empty();
		UE_LOG(FALL_DEV_LOG, Log, TEXT("BaseGameInstance :: InsGetRandomTeamLevel :: 모든 팀전 맵을 돌았으므로 초기화합니다."));
	}

	// 중복되지 않은 맵 뽑기
	FTeamLevelInfo SelectedInfo;
	do
	{
		int32 RandomIndex = FMath::RandRange(0, TeamPlayLevelInfos.Num() - 1);
		SelectedInfo = TeamPlayLevelInfos[RandomIndex];
	} while (PlayedMapList.Contains(SelectedInfo.LevelAssetName));

	// 선택된 맵 등록
	PlayedMapList.Add(SelectedInfo.LevelAssetName);

	// 팀전용 정보
	CurLevelInfo_Ins.StageLimitTime = SelectedInfo.StageLimitTime;

	// 공통 정보
	CurLevelInfo_Ins.LevelAssetName = SelectedInfo.LevelAssetName;
	CurLevelInfo_Ins.LevelName = SelectedInfo.LevelName;
	CurLevelInfo_Ins.LevelType = SelectedInfo.LevelType;
	CurLevelInfo_Ins.PlayGuide = SelectedInfo.PlayGuide;
	CurLevelInfo_Ins.GoalGuide = SelectedInfo.GoalGuide;
	CurLevelInfo_Ins.LevelIMG = SelectedInfo.LevelIMG;
	CurLevelInfo_Ins.LevelTagIMG = SelectedInfo.LevelTagIMG;

	return SelectedInfo.LevelAssetName;
}
#pragma endregion

#pragma region BaseGameInstance :: 플레이어 데이터 관련
// 플레이어 정보 백업 함수
void UBaseGameInstance::InsBackupPlayerInfo(const FString& _UniqueID, FPlayerInfo& _PlayerInfo)
{
	if (_UniqueID.IsEmpty())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsBackupPlayerInfo :: Invalid UniqueID"));
		return;
	}

	if (PlayerInfoBackup.Contains(_UniqueID))
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsBackupPlayerInfo :: 덮어쓰기 발생! 기존 정보가 사라질 수 있음 → UniqueID = %s"), *_UniqueID);
	}

	FString StatusStr = UEnum::GetValueAsString(_PlayerInfo.Status);
	PlayerInfoBackup.Add(_UniqueID, MoveTemp(_PlayerInfo));
	UE_LOG(FALL_DEV_LOG, Log, TEXT("InsBackupPlayerInfo :: UniqueID = %s, PlayerTag = %s, PlayerStatus = %s"),
		*_UniqueID, *_PlayerInfo.Tag.ToString(), *StatusStr);
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

	UE_LOG(FALL_DEV_LOG, Log, TEXT("======== PlayerInfoArray 목록 ========"));
	FString ScreenMessage = TEXT("======== PlayerInfoArray 목록 ========\n");

	for (const FPlayerInfoEntry& Entry : PlayGameState->PlayerInfoArray)
	{
		FString StatusStr = UEnum::GetValueAsString(Entry.PlayerInfo.Status);
		FString LogMessage = FString::Printf(TEXT("UniqueID: %s, Tag: %s, Status: %s, DropOrder: %d"),
			*Entry.UniqueID, *Entry.PlayerInfo.Tag.ToString(), *StatusStr, Entry.PlayerInfo.DropOrder);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("%s"), *LogMessage);
		if (GEngine) ScreenMessage += LogMessage + TEXT("\n");
	}

	UE_LOG(FALL_DEV_LOG, Log, TEXT("======== 각 플레이어의 PlayerInfo 상세 ========"));
	ScreenMessage += TEXT("======== 각 플레이어의 PlayerInfo 상세 ========\n");

	for (APlayerState* PlayerState : PlayGameState->PlayerArray)
	{
		APlayPlayerState* PS = Cast<APlayPlayerState>(PlayerState);
		if (!PS) continue;

		const FPlayerInfo& Info = PS->PlayerInfo;
		const FString StatusStr = UEnum::GetValueAsString(Info.Status);

		FString LogMessage = FString::Printf(TEXT("Tag: %s | Nick: %s | Color: %s | Top: %s | Bot: %s | Status: %s | Drop: %d | Winner: %s"),
			*Info.Tag.ToString(),
			*Info.NickName,
			*Info.CostumeColor,
			*Info.CostumeTOP,
			*Info.CostumeBOT,
			*StatusStr,
			Info.DropOrder,
			PS->GetIsWinner() ? TEXT("TRUE") : TEXT("FALSE"));

		UE_LOG(FALL_DEV_LOG, Log, TEXT("%s"), *LogMessage);
		if (GEngine) ScreenMessage += LogMessage + TEXT("\n");
	}

	if (UFallConst::PrintDebugLog && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, ScreenMessage);
	}
}

// 디버그용 : 현재 접속자 수, 카운트 다운 END 여부, 현재 스테이지 단계, 목표 골인 인원 수, 현재 골인 인원 수 확인
void UBaseGameInstance::InsPrintConnectedPlayers()
{
	APlayGameState* PlayGameState = GetWorld()->GetGameState<APlayGameState>();
	if (!PlayGameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsPrintConnectedPlayers: GameState가 nullptr 입니다."));
		if (UFallConst::PrintDebugLog && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: GameState가 nullptr 입니다."));
		}
		return;
	}

	int32 ConnectedCount = PlayGameState->GetConnectedPlayers();
	bool IsOverCount = PlayGameState->GetIsCountDownOver();
	EStagePhase StagePhase = PlayGameState->GetCurStagePhase_STATE();

	// FinishPlayer 값 가져오기
	int TargetGoalCount = PlayGameState->GetGameStateFinishPlayer();
	int CurGoalCount = PlayGameState->GetGameStateCurFinishPlayer();

	// 로그 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 접속자 수 : %d"), ConnectedCount);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("카운트 다운 끝났니? : %s"), IsOverCount ? TEXT("true") : TEXT("false"));
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 스테이지 단계 : %s"), *UEnum::GetValueAsString(StagePhase));
	UE_LOG(FALL_DEV_LOG, Log, TEXT("목표 골인 인원 수 : %d"), TargetGoalCount);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 골인 인원 수 : %d"), CurGoalCount);

	// 화면 출력
	if (UFallConst::PrintDebugLog && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("현재 접속자 수 : %d"), ConnectedCount));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("카운트 다운 끝났니? : %s"), IsOverCount ? TEXT("true") : TEXT("false")));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("현재 스테이지 단계 : %s"), *UEnum::GetValueAsString(StagePhase)));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("목표 골인 인원 수 : %d"), TargetGoalCount));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("현재 골인 인원 수 : %d"), CurGoalCount));
	}
}

// 디버그용 : 레벨 이름 확인
void UBaseGameInstance::InsPrintLevelName()
{
	APlayGameState* PlayGameState = GetWorld()->GetGameState<APlayGameState>();
	if (!PlayGameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsPrintLevelName: GameState가 nullptr 입니다."));
		if (UFallConst::PrintDebugLog && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: GameState가 nullptr 입니다."));
		}
		return;
	}

	FString LevelName = PlayGameState->GetLevelName_STATE();
	FString LevelAssetName = PlayGameState->GetLevelAssetName_STATE();

	// 콘솔 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 레벨의 이름: %s"), *LevelName);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 레벨의 에셋 이름: %s"), *LevelAssetName);

	// 화면 출력
	if (UFallConst::PrintDebugLog && GEngine)
	{
		FString ScreenMessage0 = FString::Printf(TEXT("현재 레벨의 이름: %s"), *LevelName);
		FString ScreenMessage1 = FString::Printf(TEXT("현재 레벨의 에셋 이름: %s"), *LevelAssetName);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, ScreenMessage0);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, ScreenMessage1);
	}
}

// 디버그용 : 레벨 시네마틱 바로 끝나게
void UBaseGameInstance::InsetLevelCinematicEnd()
{
	APlayGameState* PlayGameState = GetWorld()->GetGameState<APlayGameState>();
	if (!PlayGameState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("InsPrintLevelName: GameState가 nullptr 입니다."));
		if (UFallConst::PrintDebugLog && GEngine)
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
	if (UFallConst::PrintDebugLog && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("현재 레벨 시네마틱 상태 : %s"), ValueBefore ? TEXT("true") : TEXT("false")));
	}

	// TRUE로 바꿈
	PlayGameState->SetIsLevelCinematicEnd(true);

	bool ValueAfter = PlayGameState->GetIsLevelCinematicEnd();

	// 콘솔 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("레벨 시네마틱 끝났니? : %s"), ValueAfter ? TEXT("true") : TEXT("false"));

	// 화면 출력
	if (UFallConst::PrintDebugLog && GEngine)
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
		if (UFallConst::PrintDebugLog && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: GameState가 nullptr 입니다."));
		}
		return;
	}

	int TargetGoalCount = PlayGameState->GetGameStateFinishPlayer();
	int CurGoalCount = PlayGameState->GetGameStateCurFinishPlayer();

	// 콘솔 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("목표 골인 인원 : %d"), TargetGoalCount);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("현재 골인한 인원 : %d"), CurGoalCount);

	// 화면 출력
	if (UFallConst::PrintDebugLog && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("목표 골인 인원 : %d"), TargetGoalCount));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("현재 골인한 인원 : %d"), CurGoalCount));
	}
}
#pragma endregion

