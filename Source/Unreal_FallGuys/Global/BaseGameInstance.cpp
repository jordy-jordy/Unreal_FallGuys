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

	// IP 유효성 검사
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIsValidIP = false;
	Addr->SetIp(*_IP, bIsValidIP);

	if (!bIsValidIP)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("잘못된 IP 주소: %s"), *_IP);
		return;
	}

	// 포트 유효성 검사
	if (!_Port.IsNumeric())
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("포트 번호가 숫자가 아닙니다: %s"), *_Port);
		return;
	}

	int32 PortNum = FCString::Atoi(*_Port);
	if (PortNum <= 0 || PortNum > 65535)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("잘못된 포트 번호 범위: %d"), PortNum);
		return;
	}

	// 서버 연결 확인 (소켓 테스트)
	Addr->SetPort(PortNum);

	// 로컬 서버(127.0.0.1)인 경우 연결 확인을 건너뜀
	if (_IP != "127.0.0.1")
	{
		FSocket* TestSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TestSocket"), false);
		if (!TestSocket || !TestSocket->Connect(*Addr))
		{
			UE_LOG(FALL_DEV_LOG, Error, TEXT("서버 연결 실패: %s:%d"), *_IP, PortNum);
			if (TestSocket)
			{
				TestSocket->Close();
				ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(TestSocket);
			}
			return;
		}

		// 소켓 해제
		TestSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(TestSocket);
	}

	if (!_World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerConnect: _World is nullptr"));
		return;
	}

	// 연결 성공 -> 상태 업데이트
	bIsConnected = true;

	FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *_IP, *_Port);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("서버 접속 시도: %s"), *ConnectLevelName);
	UGameplayStatics::OpenLevel(_World, FName(*ConnectLevelName));
}

// 코스튬 이름 저장
void UBaseGameInstance::InsSaveCostumeName(const FString& _CostumeName)
{
	CostumeName = _CostumeName;
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("저장된 코스튬 이름 : %s"), *CostumeName);
}

// Pawn의 코스튬 변경
void UBaseGameInstance::InsChangeCostume(APawn* _Pawn, const FString& _CostumeName)
{
	UWorld* World = _Pawn->GetWorld();
	const FCostumeColorDataRow* CostumeColorData = UGlobalDataTable::GetCostumeColorData(World, _CostumeName);
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
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsChangeCostume :: Invalid Costume Data or Mesh"));
	}

	InsSaveCostumeName(_CostumeName);
}

// 저장된 코스튬의 스켈레탈 메시 반환
USkeletalMesh* UBaseGameInstance::InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName/* = TEXT("Default")*/)
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
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("InsGetCostumeMesh :: Empty Costume Data or Mesh"));
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Error, TEXT("InsGetCostumeMesh :: Invalid Costume Data or Mesh"));
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
	return MapList[RandomIndex];
}

// 리소스의 스테틱 메시 반환
UStaticMesh* UBaseGameInstance::InsGetResourceMesh(APawn* _Pawn, const FString& _MeshName/* = TEXT("Default")*/)
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
