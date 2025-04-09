// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include <Mode/01_Play/PlayEnum.h>
#include <Mode/01_Play/PlayPlayerState.h>

#include "PlayGameState.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInfoEntry
{
	GENERATED_BODY()

	// 언리얼 엔진이 지정해주는 고유 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString UniqueID;

	// 각 플레이어의 정보
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

#pragma region PlayGameState :: 플레이어 관련
public:
	// 전체 플레이어 정보 목록
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "PLAYER LIST")
	TArray<FPlayerInfoEntry> PlayerInfoArray;

	// 플레이어 정보 캐싱용 맵 (UID → PlayerInfo)
	TMap<FString, FPlayerInfo> CachedPlayerInfoMap;

	// 실패한 플레이어 정보 목록
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "PLAYER LIST")
	TArray<FPlayerInfoEntry> FailPlayerInfoArray;

	// `PlayerState`에서 데이터 동기화
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PLAYER LIST")
	void SyncPlayerInfoFromPlayerState();
	void SyncPlayerInfoFromPlayerState_Implementation();

	// 접속자 수 증가
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYERS")
	void AddConnectedPlayers();
	void AddConnectedPlayers_Implementation();

	// 접속자 수 감소
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYERS")
	void MinusConnectedPlayers();
	void MinusConnectedPlayers_Implementation();

	// 접속자 수 동기화
	UFUNCTION(Reliable, NetMulticast, Category = "PLAYERS")
	void MulticastUpdateConnectedPlayers(int _NewCount);
	void MulticastUpdateConnectedPlayers_Implementation(int _NewCount);

	// 현재 접속한 플레이어 수 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYERS")
	int GetConnectedPlayers() const { return ConnectedPlayers; }

	// 실패한 유저의 떨어지는 순번을 정해줌
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PLAYERS")
	void SetDropOrder();
	void SetDropOrder_Implementation();


protected:
	// 접속한 플레이어의 수
	UPROPERTY(Replicated)
	int ConnectedPlayers = 0;

#pragma endregion

#pragma region PlayGameState :: 레벨 데이터 관련
public:
	// 레벨 이름 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetPlayLevelName(const FString& _LevelName);
	void SetPlayLevelName_Implementation(const FString& _LevelName);

	// 레벨 에셋 이름 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetPlayLevelAssetName(const FString& _LevelName);
	void SetPlayLevelAssetName_Implementation(const FString& _LevelAssetName);

	// 레벨 타입 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetCurStageType(EStageType _StageType);
	void SetCurStageType_Implementation(EStageType _StageType);

	// 레벨 페이즈 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetCurStagePhase(EStagePhase _StagePhase);
	void SetCurStagePhase_Implementation(EStagePhase _StagePhase);

	// 세팅된 레벨 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetLevelName() const { return LevelName; }

	// 세팅된 레벨 에셋 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetLevelAssetName() const { return LevelAssetName; }

	// 세팅된 레벨 타입 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	EStageType GetCurStageType() const { return GS_CurStageType; }

	// 세팅된 레벨 단계 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	EStagePhase GetCurStagePhase() const { return GS_CurStagePhase; }

	// 레벨 시네마틱 시작하세요
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetCanStartLevelCinematic();
	void SetCanStartLevelCinematic_Implementation();

	// 레벨 시네마틱 시작해도 되나요?
	bool GetCanStartLevelCinematic() { return CanStartLevelCinematic; }

	// 레벨 시네마틱 끝났어요
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetIsLevelCinematicEnd(bool _Value);
	void SetIsLevelCinematicEnd_Implementation(bool _Value);

	// 레벨 시네마틱 끝났나요?
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	bool GetIsLevelCinematicEnd() { return IsLevelCinematicEnd; }

	// 골인 목표 인원 수 세팅 완료 했는지
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateSettedGoalCountTrue(bool _Value);
	void SetGameStateSettedGoalCountTrue_Implementation(bool _Value);

	// 골인 목표 인원 수 세팅 됐는지 확인할게
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	bool GetGameStateSettedGoalCount() { return bGameStateSettedGoalCount; }

	// 골인 목표 인원 수 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateFinishPlayer(int _Value);
	void SetGameStateFinishPlayer_Implementation(int _Value);
	
	// 골인 목표 인원 수 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	int GetGameStateFinishPlayer() { return GameStateFinishPlayer; }

	// 현재 골인한 플레이어 수 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateCurFinishPlayer(int _Value);
	void SetGameStateCurFinishPlayer_Implementation(int _Value);

	// 현재 골인한 플레이어 수 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	int GetGameStateCurFinishPlayer() { return GameStateCurFinishPlayer; }

	// 결과 화면인지 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateIsResultLevel(bool _Value);
	void SetGameStateIsResultLevel_Implementation(bool _Value);

	// 결과 화면인지 반환
	bool GetGameStateIsResultLevel() { return bGameStateIsResultLevel; }

	// 게임 시작했음을 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateGameStarted(bool _Value);
	void SetGameStateGameStarted_Implementation(bool _Value);

	// 게임 시작했니?
	bool GetGameStateGameStarted() { return bGameStateGameStarted; }

protected:
	// 랜덤 레벨 네임
	UPROPERTY(Replicated)
	FString LevelName = TEXT("");

	// 랜덤 레벨 에셋 네임
	UPROPERTY(Replicated)
	FString LevelAssetName = TEXT("");

	// 현재 스테이지 타입
	UPROPERTY(Replicated)
	EStageType GS_CurStageType = EStageType::NONE;

	// 현재 스테이지 페이즈
	UPROPERTY(Replicated)
	EStagePhase GS_CurStagePhase = EStagePhase::STAGE_1;

	// 레벨 시네마틱 시작해도 되니?
	UPROPERTY(Replicated)
	bool CanStartLevelCinematic = false;

	// 레벨 시네마틱 끝났니?
	UPROPERTY(Replicated)
	bool IsLevelCinematicEnd = false;

	// 골인 목표 인원 수 조정 했니?
	UPROPERTY(Replicated)
	bool bGameStateSettedGoalCount = false;

	// 골인 목표 인원 수 : PlayGameMode에서 가져옴
	UPROPERTY(Replicated)
	int GameStateFinishPlayer = 99;

	// 현재 골인한 플레이어 수 : PlayGameMode에서 가져옴
	UPROPERTY(Replicated)
	int GameStateCurFinishPlayer = 0;

	// 결과 화면이니?
	UPROPERTY(Replicated)
	bool bGameStateIsResultLevel = false;

	// 게임 시작했니?
	UPROPERTY(Replicated)
	bool bGameStateGameStarted = false;

#pragma endregion

#pragma region PlayGameState :: 카운트 다운 관련
public:
	// 카운트 다운 시간 반환
	UFUNCTION(BlueprintCallable, Category = "COUNT DOWN")
	float GetCountDownTime() { return CountDownTime; }

	// CountDownTime 설정 (정상적인 경우 사용할 일 없음)
	UFUNCTION(BlueprintCallable, Category = "COUNT DOWN")
	void SetCountDownTime(float _Time) { CountDownTime = _Time; }

	// CountDownTime 에서 Value 차감 : PlayGameMode에서 호출
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "COUNT DOWN")
	void MinusCountDownTime(float _Value);
	void MinusCountDownTime_Implementation(float _Value);

	// 카운트 다운이 끝났는지 확인하는 함수
	UFUNCTION(BlueprintCallable, Category = "COUNT DOWN")
	bool GetIsCountDownOver() { return IsCountDownOver; }

	// 카운트 다운이 끝났음을 알림 : PlayGameMode에서 호출
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "COUNT DOWN")
	void SetIsCountDownOverTrue();
	void SetIsCountDownOverTrue_Implementation();

protected:
	// 카운트 다운 시간 : FallConst로부터 가져옴
	UPROPERTY(Replicated)
	float CountDownTime;

	// 카운트다운이 끝났는지 확인
	UPROPERTY(Replicated)
	bool IsCountDownOver = false;

#pragma endregion


#pragma region PlayGameState :: 동기화 관련
protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion

// 디버그용
public:
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void PrintFailPlayersInfo();



};