// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include <Mode/01_Play/PlayPlayerState.h>

#include "BaseGameInstance.generated.h"


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

	// 저장된 코스튬의 컬러 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	FString InsGetCostumeColor() const
	{
		return CostumeColor;
	}

	// 저장된 코스튬의 상의 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	FString InsGetCostumeTop() const
	{
		return CostumeTop;
	}

	// 저장된 코스튬의 하의 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	FString InsGetCostumeBot() const
	{
		return CostumeBot;
	}

	// 코스튬 컬러 저장
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsSaveCostumeColor(const FString& _CostumeColor);

	// 코스튬 상의 저장
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsSaveCostumeTop(const FString& _CostumeTop);

	// 코스튬 하의 저장
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsSaveCostumeBot(const FString& _CostumeBot);

	// Pawn의 코스튬 컬러 변경
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsChangeCostumeColor(APawn* _Pawn, const FString& _CostumeColor);

	// Pawn의 코스튬 상의 변경
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsChangeCostumeTop(APawn* _Pawn, UStaticMeshComponent* _UpComp, const FString& _CostumeTop);

	// Pawn의 코스튬 하의 변경
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsChangeCostumeBot(APawn* _Pawn, UStaticMeshComponent* _LowComp, const FString& _CostumeBot);

	// 저장된 코스튬 컬러의 스켈레탈 메시 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	USkeletalMesh* InsGetCostumeColorMesh(APawn* _Pawn, const FString& _MeshName = TEXT("Default"));

	// 저장된 코스튬의 스테틱 메시 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	UStaticMesh* InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName = TEXT("Default"));

	// 리소스의 스테틱 메시 반환
	UFUNCTION(BlueprintCallable, Category = "RESOURCE")
	UStaticMesh* InsGetResourceMesh(APawn* _Pawn, const FString& _MeshName);

	// 저장된 닉네임 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER NICKNAME")
	FString InsGetNickname() const
	{
		return Nickname;
	}

	// 닉네임 변경
	UFUNCTION(BlueprintCallable, Category = "PLAYER NICKNAME")
	void InsChangeNickname(const FString& _NewNickname);

	// Random PlayLevel의 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString InsGetRandomLevel();

	// 저장된 레벨의 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString InsGetCurLevelName() const
	{
		return CurLevelName;
	}

	// 레벨 이동했는지 체크하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PLAYER DATA")
	bool IsMovedLevel = false;

	// 플레이어 정보 백업 함수
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	void InsBackupPlayerInfo(const FString& _UniqueID, const FPlayerInfo& _PlayerInfo);

	// 백업된 플레이어 정보 가져오기 함수
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	bool InsGetBackedUpPlayerInfo(const FString& _UniqueID, FPlayerInfo& _OutPlayerInfo) const;

	// 서버 연결 상태를 TRUE로 전환
	UFUNCTION(BlueprintCallable, Category = "SERVER")
	void InsSetbIsConnectedTrue()
	{
		bIsConnected = true;
	}

protected:
	// 플레이 레벨 데이터 테이블을 얻는 함수
	UFUNCTION(BlueprintCallable, Category = "DATA")
	class UDataTable* GetPlayLevelDataTable() const;

	// 플레이어 UniqueID -> FPlayerInfo 매핑 저장소
	TMap<FString, struct FPlayerInfo> PlayerInfoBackup;

private:
	UFUNCTION(BlueprintCallable, Category = "SERVER")
	void CServerStart(UWorld* _World, FString _Port);

	UFUNCTION(BlueprintCallable, Category = "SERVER")
	void InsSelectedServerStart(UWorld* _World, FString _Port, FString _OpenLevel);

	UFUNCTION(BlueprintCallable, Category = "SERVER")
	void CServerConnect(UWorld* _World, FString _IP, FString _Port);

	UPROPERTY(VisibleAnywhere, Category = "DATA")
	class UDataTable* DataTables = nullptr;
	class UDataTable* CostumeDataTable = nullptr;
	class UDataTable* CostumeColorDataTable = nullptr;
	class UDataTable* PlayLevelDataTable = nullptr;
	class UDataTable* ResourceDataTable = nullptr;
	//class UDataTable* ActorDataTable = nullptr;

	// 코스튬 컬러
	UPROPERTY(VisibleAnywhere, Category = "PLAYER COSTUME")
	FString CostumeColor = TEXT("Default");

	// 코스튬 상의
	UPROPERTY(VisibleAnywhere, Category = "PLAYER COSTUME")
	FString CostumeTop = TEXT("Default_Top");

	// 코스튬 하의
	UPROPERTY(VisibleAnywhere, Category = "PLAYER COSTUME")
	FString CostumeBot = TEXT("Default_Bot");

	// 닉네임
	UPROPERTY(VisibleAnywhere, Category = "PLAYER NICKNAME")
	FString Nickname = TEXT("TEST_JORDY");

	// 레벨 이름
	UPROPERTY(VisibleAnywhere, Category = "LEVEL")
	FString CurLevelName = TEXT("");

	// 맵리스트
	TArray<FString> MapList;
	TSet<int> PlayedMapList;

	// 서버 연결 상태 변수
	bool bIsConnected = false;

	// 디버그용 : 플레이어 태그 확인
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void InsPrintPlayerInfo();

	// 디버그용 : 접속자 수 확인
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void InsPrintConnectedPlayers();

	// 디버그용 : 레벨 이름 확인
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void InsPrintLevelName();

//LMH
public:
	UFUNCTION(BlueprintCallable)
	bool GetIsDie() { return IsDie; } const

	UFUNCTION(BlueprintCallable)
	void SetIsDie(bool _val)
	{
		IsDie = _val;
	}

	UPROPERTY(BlueprintReadWrite)
	bool IsDie = true;
};

