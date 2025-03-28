// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include <Mode/01_Play/PlayPlayerState.h>

#include "PlayGameState.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInfoEntry
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString UniqueID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FPlayerInfo PlayerInfo;

	FPlayerInfoEntry() : UniqueID(TEXT("")), PlayerInfo()
	{
	}

	FPlayerInfoEntry(const FString& _InUniqueID, const FPlayerInfo& _InInfo)
		: UniqueID(_InUniqueID), PlayerInfo(_InInfo)
	{
	}
};

/**
 *
 */
UCLASS()
class UNREAL_FALLGUYS_API APlayGameState : public AGameState
{
	GENERATED_BODY()

public:
	APlayGameState();
	virtual void BeginPlay() override;

	// 전체 플레이어 정보 목록 (GameState에서 관리)
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "PLAYER LIST")
	TArray<FPlayerInfoEntry> PlayerInfoArray;

	// `PlayerState`에서 데이터 동기화
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYER LIST")
	void SyncPlayerInfoFromPlayerState();
	void SyncPlayerInfoFromPlayerState_Implementation();

	// 접속자 수 증가
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYERS")
	void AddConnectedPlayers();
	void AddConnectedPlayers_Implementation();

	// 접속자 수 동기화
	UFUNCTION(Reliable, NetMulticast, Category = "PLAYERS")
	void MulticastUpdateConnectedPlayers(int _NewCount);
	void MulticastUpdateConnectedPlayers_Implementation(int _NewCount);

	// 현재 접속한 플레이어 수 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYERS")
	int GetConnectedPlayers() const { return ConnectedPlayers; }

	// 카운트다운 시간 (서버에서 클라이언트로 동기화됨)
	UPROPERTY(Replicated)
	float CountDownTime;

	// 카운트다운이 끝났는지 확인하는 변수
	UPROPERTY(Replicated)
	bool IsCountDownOver = false;

	// 게임 인스턴스에서 세팅된 레벨 이름
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SavePlayLevelName(const FString& _LevelName);
	void SavePlayLevelName_Implementation(const FString& _LevelName);

	// 게임 인스턴스에서 세팅된 레벨 에셋 이름
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SavePlayLevelAssetName(const FString& _LevelName);
	void SavePlayLevelAssetName_Implementation(const FString& _LevelAssetName);

	// 랜덤 레벨 함수에서 얻은 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetLevelName() const
	{
		return LevelName;
	}

	// 랜덤 레벨 함수에서 얻은 에셋 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetLevelAssetName() const
	{
		return LevelAssetName;
	}

	// Stage 제한 시간 유무 - 외부에서 값 가져옴
	UPROPERTY(Replicated)
	bool UseStageLimitTime;

	// Stage 제한 시간 - 외부에서 값 가져옴
	UPROPERTY(Replicated)
	float StageLimitTime;

	UFUNCTION()
	void OnRep_ConnectedPlayers();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// 접속한 플레이어의 수
	UPROPERTY(ReplicatedUsing = OnRep_ConnectedPlayers)
	int ConnectedPlayers = 0;

	// 랜덤 레벨 네임
	UPROPERTY(Replicated)
	FString LevelName = TEXT("");

	// 랜덤 레벨 에셋 네임
	UPROPERTY(Replicated)
	FString LevelAssetName = TEXT("");

	// Stage 제한 시간 유무 결정 함수
	bool SetUseStageLimitTime() const;

	// Stage 제한 시간 결정 함수
	float SetStageLimitTime() const;

//LMHH
public:
	UFUNCTION(BlueprintCallable)
	void SpawnEggManager();

	UPROPERTY(EditAnywhere, Category = "Manager")
	TSubclassOf<class AEggSpawnManager> SpawnManagerFactory;
	
	UPROPERTY(BlueprintReadWrite, Category = "Manager")
	class AEggSpawnManager* Spawnner;


};