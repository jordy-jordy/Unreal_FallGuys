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
#include <Mode/01_Play/PlayGameState.h>
#include <Mode/01_Play/PlayPlayerState.h>


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

			// 리소스 데이터 로드
			ResourceDataTable = DataTables->FindRow<FDataTableRow>("DT_ResourceDataTable", nullptr)->Resources;
			if (nullptr == ResourceDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == ResourceDataTable)"), __FUNCTION__, __LINE__);
			}
		}

		//if (nullptr != DataTables)
		//{
		//	ActorDataTable = DataTables->FindRow<FDataTableRow>("DT_GlobalActorDataTable", nullptr)->Resources;
		//	if (nullptr == ActorDataTable)
		//	{
		//		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == ActorDataTable)"), __FUNCTION__, __LINE__);
		//	}
		//}
	}
	// 흰색
	UE_LOG(FALL_DEV_LOG, Log, TEXT("%S(%u)> DataTableLoading End"), __FUNCTION__, __LINE__);
}

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
		// _Pawn의 스켈레탈 메시 가져오기
		if (USkeletalMeshComponent* MeshComp = _Pawn->FindComponentByClass<USkeletalMeshComponent>())
		{
			MeshComp->SetSkeletalMesh(CostumeColorData->CostumeMesh);
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostume :: Invalid Costume Color Data or Mesh"));
	}

	InsSaveCostumeColor(_CostumeColor);
}

// Pawn의 코스튬 상의 변경
void UBaseGameInstance::InsChangeCostumeTop(APawn* _Pawn, UStaticMeshComponent* _UpComp, const FString& _CostumeTop)
{
	if (_CostumeTop == TEXT("Default") || _CostumeTop == TEXT(""))
	{
		if (_UpComp)
		{
			_UpComp->SetStaticMesh(nullptr);
			UE_LOG(FALL_DEV_LOG, Log, TEXT("InsChangeCostumeTop :: Default 감지, 메시 제거"));
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostumeTop :: _UpComp is nullptr"));
		}

		InsSaveCostumeTop(_CostumeTop);
		return;
	}

	UWorld* World = _Pawn->GetWorld();
	const FCostumeDataRow* CostumeTopData = UGlobalDataTable::GetCostumeData(World, _CostumeTop);

	if (CostumeTopData && CostumeTopData->CostumeMesh)
	{
		if (_UpComp)
		{
			_UpComp->SetStaticMesh(CostumeTopData->CostumeMesh);
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostumeTop :: _UpComp is nullptr"));
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostumeTop :: Invalid Costume Top Data or Mesh"));
	}

	InsSaveCostumeTop(_CostumeTop);
}

// Pawn의 코스튬 하의 변경
void UBaseGameInstance::InsChangeCostumeBot(APawn* _Pawn, UStaticMeshComponent* _LowComp, const FString& _CostumeBot)
{
	if (_CostumeBot == TEXT("Default") || _CostumeBot == TEXT(""))
	{
		if (_LowComp)
		{
			_LowComp->SetStaticMesh(nullptr);
			UE_LOG(FALL_DEV_LOG, Log, TEXT("InsChangeCostumeBot :: Default 감지, 메시 제거"));
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostumeBot :: _LowComp is nullptr"));
		}

		InsSaveCostumeBot(_CostumeBot);
		return;
	}

	UWorld* World = _Pawn->GetWorld();
	const FCostumeDataRow* CostumeBotData = UGlobalDataTable::GetCostumeData(World, _CostumeBot);
	if (CostumeBotData && CostumeBotData->CostumeMesh)
	{
		if (_LowComp)
		{
			_LowComp->SetStaticMesh(CostumeBotData->CostumeMesh);
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostumeBot :: _LowComp is nullptr"));
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostumeBot :: Invalid Costume Bot Data or Mesh"));
	}

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
		else
		{
			UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetCostumeMesh :: Invalid Costume  Color Mesh Data or Mesh"));
		}
	}

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
		else
		{
			UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetCostumeMesh :: Invalid Costume Mesh Data or Mesh"));
		}
	}

	return nullptr;
}

// 리소스의 스테틱 메시 반환
UStaticMesh* UBaseGameInstance::InsGetResourceMesh(APawn* _Pawn, const FString& _MeshName)
{
	const FResourceDataRow* ResourceData = UGlobalDataTable::GetResourceData(_Pawn->GetWorld(), _MeshName);
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

// 닉네임 변경
void UBaseGameInstance::InsChangeNickname(const FString& _NewNickname)
{
	Nickname = _NewNickname;
}

// Random PlayLevel의 이름 반환
FString UBaseGameInstance::InsGetRandomLevel()
{
	if (MapList.Num() == 0)
	{
		MapList = UFallGlobal::GetAvailableLevels(); // 중복 추가 방지
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

	LevelName = MapList[RandomIndex];

	return LevelName;
}

// 플레이 레벨 데이터 테이블을 얻는 함수
UDataTable* UBaseGameInstance::GetPlayLevelDataTable() const
{
	return PlayLevelDataTable; 
}

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

// 백업된 플레이어 정보 가져오기 함수
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

// 디버그용
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
		FString LogMessage = FString::Printf(TEXT("UniqueID: %s, Tag: %s, Status: %d"),
			*Entry.UniqueID, *Entry.PlayerInfo.Tag, static_cast<int32>(Entry.PlayerInfo.Status));

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
			FString LogMessage = FString::Printf(TEXT("UniqueID: %s, Tag: %s, Status: %d"),
				*PlayPlayerState->PlayerInfo.UniqueID, *PlayPlayerState->PlayerInfo.Tag,
				static_cast<int32>(PlayPlayerState->PlayerInfo.Status));

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

