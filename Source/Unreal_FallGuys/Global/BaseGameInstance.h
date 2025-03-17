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

	// 코스튬 이름 저장
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsSaveCostumeName(const FString& _CostumeName);

	// Pawn의 코스튬 변경
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsChangeCostume(APawn* _Pawn, const FString& _CostumeName);

	// 저장된 코스튬의 이름 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	FString InsGetCostumeName() const
	{
		return CostumeName;
	}

	// 저장된 코스튬의 스켈레탈 메시 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	USkeletalMesh* InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName = TEXT("Default"));

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

	// 리소스의 스테틱 메시 반환
	UFUNCTION(BlueprintCallable, Category = "RESOURCE")
	UStaticMesh* InsGetResourceMesh(APawn* _Pawn, const FString& _MeshName = TEXT("Default"));

	// 레벨 이동했는지 체크하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PLAYER DATA")
	bool IsMovedLevel = false;

	// 플레이어 정보 백업 함수
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	void InsBackupPlayerInfo(const FString& _UniqueID, const FPlayerInfo& _PlayerInfo);

	// 백업된 플레이어 정보 가져오기 함수
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	bool InsGetBackedUpPlayerInfo(const FString& _UniqueID, FPlayerInfo& _OutPlayerInfo) const;

	// 랜덤 레벨 함수에서 얻은 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetLevelName()
	{
		return LevelName;
	}

	UFUNCTION(BlueprintCallable, Category = "SERVER")
	void SetbIsConnectedTrue()
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

	// 코스튬 네임
	UPROPERTY(VisibleAnywhere, Category = "PLAYER COSTUME")
	FString CostumeName = TEXT("Default");

	// 닉네임
	UPROPERTY(VisibleAnywhere, Category = "PLAYER NICKNAME")
	FString Nickname = TEXT("TEST_JORDY");

	// 랜덤 레벨 네임
	UPROPERTY(VisibleAnywhere, Category = "LEVEL")
	FString LevelName = TEXT("");

	// 맵리스트
	TArray<FString> MapList;
	TSet<int> PlayedMapList;

	// 서버 연결 상태 변수
	bool bIsConnected = false;

	// 디버그용
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void InsPrintPlayerInfo();

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
	bool IsDie = false;
};

