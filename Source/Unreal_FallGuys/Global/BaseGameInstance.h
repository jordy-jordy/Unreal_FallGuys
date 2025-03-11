// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Common/UdpSocketBuilder.h"
#include "IPAddress.h"

#include <Global/Data/GlobalDataTable.h>

#include "BaseGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString IP;

	UPROPERTY(BlueprintReadWrite)
	FString Port;
};

/**
 *
 */
UCLASS()
class UNREAL_FALLGUYS_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	friend class UGlobalDataTable;
	friend class UFallGlobal;

public:
	UBaseGameInstance();

	// 코스튬 이름 저장
	UFUNCTION(BlueprintCallable, Category = "Costume")
	void InsSaveCostumeName(const FString& _CostumeName);

	// Pawn의 코스튬 변경
	UFUNCTION(BlueprintCallable, Category = "Costume")
	void InsChangeCostume(APawn* _Pawn, const FString& _CostumeName);

	// 저장된 코스튬의 이름 반환
	UFUNCTION(BlueprintCallable, Category = "Costume")
	FString InsGetCostumeName() const
	{
		return CostumeName;
	}

	// 저장된 코스튬의 스켈레탈 메시 반환
	UFUNCTION(BlueprintCallable, Category = "Costume")
	USkeletalMesh* InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName = TEXT("NULL"));

	// 저장된 닉네임 반환
	UFUNCTION(BlueprintCallable, Category = "Name")
	FString InsGetNickname() const
	{
		return Nickname;
	}

	// 닉네임 변경
	UFUNCTION(BlueprintCallable, Category = "Name")
	void InsChangeNickname(const FString& _NewNickname);

	// 동기화 변수
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	// 서버 정보 브로드캐스트 (UDP)
	void InsStartServerBroadcast(FString _Port);
	void InsSendBroadcast();

	// 클라이언트가 서버 리스트를 받기 위한 함수
	void InsStartListeningForServers();
	bool InsReceiveBroadcast(float _DeltaTime);
	void InsParseAndStoreServerInfo(FString _Message);

	// 서버 리스트 반환 (UI에서 사용 가능)
	UFUNCTION(BlueprintCallable, Category = "Server")
	TArray<FServerInfo> InsGetServerList();

	UFUNCTION(BlueprintCallable, Category = "Network")
	FString InsGetLocalIP();

protected:
	UFUNCTION(BlueprintCallable, Category = "DataTable")
	UDataTable* GetPlayLevelDataTable() const { return PlayLevelDataTable; }


private:
	UFUNCTION(BlueprintCallable, Category = "Server")
	void CServerStart(UWorld* _World, FString _Port);

	UFUNCTION(BlueprintCallable, Category = "Server")
	void InsSelectedServerStart(UWorld* _World, FString _Port, FString _OpenLevel);

	UFUNCTION(BlueprintCallable, Category = "Server")
	void CServerConnect(UWorld* _World, FString _IP, FString _Port);

	UPROPERTY(VisibleAnywhere, Category = "Data")
	class UDataTable* DataTables = nullptr;
	class UDataTable* CostumeDataTable = nullptr;
	class UDataTable* PlayLevelDataTable = nullptr;
	//class UDataTable* ActorDataTable = nullptr;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Costume")
	FString CostumeName = TEXT("NULL");

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Name")
	FString Nickname = TEXT("TEST_JORDY");

	TArray<FServerInfo> ServerList;

	// 네트워크 관련 변수
	FSocket* SenderSocket;
	FSocket* ReceiverSocket;
	FTimerHandle BroadcastTimerHandle;
	FIPv4Address BroadcastAddress;
	FIPv4Endpoint BroadcastEndpoint;
};
