// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include <Global/Data/GlobalDataTable.h>
#include <Global/GlobalEnum.h>
#include <Mode/01_Play/PlayGameState.h>
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
	USkeletalMesh* InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName = TEXT("NULL"));

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
	UStaticMesh* InsGetResourceMesh(APawn* _Pawn, const FString& _MeshName = TEXT("NULL"));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	bool IsMovedLevel = false;

protected:
	UFUNCTION(BlueprintCallable, Category = "DATA")
	UDataTable* GetPlayLevelDataTable() const { return PlayLevelDataTable; }


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
	FString CostumeName = TEXT("NULL");

	// 닉네임
	UPROPERTY(VisibleAnywhere, Category = "PLAYER NICKNAME")
	FString Nickname = TEXT("TEST_JORDY");

	// 맵리스트
	TArray<FString> MapList;
	TSet<int> PlayedMapList;

	// 서버 연결 상태 변수
	bool bIsConnected = false;

};

