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
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateConnectedPlayers(int _NewCount);
	void MulticastUpdateConnectedPlayers_Implementation(int _NewCount);

	// 현재 접속한 플레이어 수 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYERS")
	int GetConnectedPlayers() const { return ConnectedPlayers; }

	// 카운트다운 시간 (서버에서 클라이언트로 동기화됨)
	UPROPERTY(Replicated)
	float CountDownTime;

	// 게임 인스턴스에서 세팅된 레벨 이름
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SavePlayLevelName(const FString& _LevelName);
	void SavePlayLevelName_Implementation(const FString& _LevelName);

	// 랜덤 레벨 함수에서 얻은 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetLevelName() const
	{
		return LevelName;
	}

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

};