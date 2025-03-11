// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "Containers/Ticker.h"
#include <Net/UnrealNetwork.h>

#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/FallConst.h>


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
			CostumeDataTable = DataTables->FindRow<FDataTableRow>("DT_CostumeDataTable", nullptr)->Resources;
			if (nullptr == CostumeDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == CostumeDataTable)"), __FUNCTION__, __LINE__);
			}

			PlayLevelDataTable = DataTables->FindRow<FDataTableRow>("DT_PlayLevelDataTable", nullptr)->Resources;
			if (nullptr == PlayLevelDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == PlayLevelDataTable)"), __FUNCTION__, __LINE__);
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
//void UBaseGameInstance::CServerStart(UWorld* _World, FString _Port)
//{
//	if (!_World)
//	{
//		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerStart: _World is nullptr"));
//		return;
//	}
//
//	FString OpenLevel;
//	FString LevelPath = TEXT("");
//
//	UFallGlobal::AssetPackagePath(UWorld::StaticClass(), UFallConst::PlayLevelName, LevelPath);
//	OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);
//
//	UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
//}

void UBaseGameInstance::CServerStart(UWorld* _World, FString _Port)
{
	FString LevelPath = TEXT("");
	UFallGlobal::AssetPackagePath(UWorld::StaticClass(), UFallConst::PlayLevelName, LevelPath);
	FString OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);

	// 서버 정보 브로드캐스트 시작 (UDP)
	InsStartServerBroadcast(_Port);

	// OpenLevel로 서버 시작
	UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
}

// 서버 오픈 : 레벨 선택 필요
void UBaseGameInstance::InsSelectedServerStart(UWorld* _World, FString _Port, FString _OpenLevel)
{
	if (!_World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerStart: _World is nullptr"));
		return;
	}

	FString OpenLevel;
	FString LevelPath = TEXT("");

	UFallGlobal::AssetPackagePath(UWorld::StaticClass(), _OpenLevel, LevelPath);
	OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);

	// 서버 정보 브로드캐스트 시작 (UDP)
	InsStartServerBroadcast(_Port);

	UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
}

// 서버 접속
void UBaseGameInstance::CServerConnect(UWorld* _World, FString _IP, FString _Port)
{
	if (!_World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerConnect: _World is nullptr"));
		return;
	}

	FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *_IP, *_Port);

	UGameplayStatics::OpenLevel(_World, FName(*ConnectLevelName));
}

// 동기화 변수
void UBaseGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseGameInstance, CostumeName);
	DOREPLIFETIME(UBaseGameInstance, Nickname);
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
	const FCostumeDataRow* CostumeData = UGlobalDataTable::GetCostumeData(World, _CostumeName);
	if (CostumeData && CostumeData->CostumeMesh)
	{
		// _Pawn의 스켈레탈 메시 가져오기
		if (USkeletalMeshComponent* MeshComp = _Pawn->FindComponentByClass<USkeletalMeshComponent>())
		{
			MeshComp->SetSkeletalMesh(CostumeData->CostumeMesh);
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("ChangeCostume :: Invalid Costume Data or Mesh"));
	}

	InsSaveCostumeName(_CostumeName);
}

// 저장된 코스튬의 스켈레탈 메시 반환
USkeletalMesh* UBaseGameInstance::InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName/* = TEXT("NULL")*/)
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

// 닉네임 변경
void UBaseGameInstance::InsChangeNickname(const FString& _NewNickname)
{
	Nickname = _NewNickname;
}

void UBaseGameInstance::InsStartServerBroadcast(FString _Port)
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("서버 브로드 캐스트가 시작되었습니다."));

	// 클래스 멤버 변수 BroadcastEndpoint를 사용하도록 수정
	BroadcastEndpoint = FIPv4Endpoint(FIPv4Address(255, 255, 255, 255), FCString::Atoi(*_Port));

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	SenderSocket = FUdpSocketBuilder(TEXT("ServerBroadcastSocket"))
		.AsReusable()
		.WithBroadcast()
		.WithSendBufferSize(2 * 1024);

	if (SenderSocket)
	{
		FIPv4Address::Parse("255.255.255.255", BroadcastAddress);
		SenderSocket->SetBroadcast(true);

		// 타이머 설정을 유지
		GetWorld()->GetTimerManager().SetTimer(BroadcastTimerHandle, this, &UBaseGameInstance::InsSendBroadcast, 1.0f, true);
	}
}

// 서버 정보 브로드캐스트 전송 (1초마다 실행됨)
void UBaseGameInstance::InsSendBroadcast()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("브로드 캐스트 Send."));

	FString Message = FString::Printf(TEXT("ServerInfo:%s:%s"), *InsGetLocalIP(), *FString::FromInt(7777));

	// UTF-8 변환을 통해 Null 바이트를 제거한 후 패킷 생성
	FTCHARToUTF8 Converter(*Message);
	TArray<uint8> Data;
	Data.Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());

	int32 BytesSent = 0;

	if (SenderSocket)
	{
		SenderSocket->SendTo(Data.GetData(), Data.Num(), BytesSent, *BroadcastEndpoint.ToInternetAddr());
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("SendBroadcast: SenderSocket is null"));
	}
}

// 클라이언트가 서버 리스트 수신을 시작 (BeginPlay()에서 호출)
void UBaseGameInstance::InsStartListeningForServers()
{
	FIPv4Endpoint ListenEndpoint(FIPv4Address::Any, 7777);
	ReceiverSocket = FUdpSocketBuilder(TEXT("ServerReceiverSocket"))
		.AsReusable()
		.BoundToEndpoint(ListenEndpoint)
		.WithReceiveBufferSize(2 * 1024);

	if (ReceiverSocket)
	{
		FTSTicker* Ticker = &FTSTicker::GetCoreTicker();

		// nullptr 체크로 변경
		if (Ticker)
		{
			Ticker->AddTicker(
				FTickerDelegate::CreateUObject(this, &UBaseGameInstance::InsReceiveBroadcast),
				1.0f // 실행 주기 (초 단위)
			);
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Error, TEXT("FTSTicker::GetCoreTicker() returned nullptr!"));
		}
	}
}

// 서버 브로드캐스트 메시지를 받아 서버 리스트 업데이트
bool UBaseGameInstance::InsReceiveBroadcast(float DeltaTime)
{
	if (!ReceiverSocket)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("ReceiveBroadcast: ReceiverSocket is null!"));
		return false;
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("ReceiveBroadcast: ReceiverSocket Received"));
	}

	uint32 Size;
	while (ReceiverSocket->HasPendingData(Size))
	{
		TArray<uint8> ReceivedData;
		ReceivedData.SetNumUninitialized(Size);

		int32 BytesRead = 0;
		ReceiverSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), BytesRead);

		// Null 바이트 제거
		while (!ReceivedData.IsEmpty() && ReceivedData.Last() == 0)
		{
			ReceivedData.Pop();
		}

		FString ReceivedMessage = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
		UE_LOG(FALL_DEV_LOG, Log, TEXT("Received Broadcast: %s"), *ReceivedMessage);

		InsParseAndStoreServerInfo(ReceivedMessage);
	}

	return true;
}

// 받은 메시지를 파싱하여 서버 리스트에 저장
void UBaseGameInstance::InsParseAndStoreServerInfo(FString Message)
{
	if (Message.StartsWith("ServerInfo:"))
	{
		FString IP, Port;
		Message.RemoveFromStart("ServerInfo:");
		Message.Split(":", &IP, &Port);

		FServerInfo NewServer;
		NewServer.IP = IP;
		NewServer.Port = Port;

		ServerList.Add(NewServer);
	}
}

// 서버 리스트를 가져오는 함수 (UI에서 사용 가능)
TArray<FServerInfo> UBaseGameInstance::InsGetServerList()
{
	return ServerList;
}

// 현재 컴퓨터의 로컬 IP 주소 반환 함수
FString UBaseGameInstance::InsGetLocalIP()
{
	bool bCanBind = false;
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	// `TSharedRef` 대신 `TSharedPtr` 사용
	TSharedPtr<FInternetAddr> LocalAddr = SocketSubsystem->GetLocalHostAddr(*GLog, bCanBind);

	if (LocalAddr && LocalAddr->IsValid()) // 올바른 null 체크 방식
	{
		return LocalAddr->ToString(false); // `false`로 하면 포트 제외하고 IP만 반환
	}

	return TEXT("127.0.0.1"); // 오류 발생 시 기본값 (루프백 주소)
}
