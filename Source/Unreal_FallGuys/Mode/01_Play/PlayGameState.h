// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

// 위젯 델리게이트 타입 포함
#include <Global/FallGlobal.h>

#include <Mode/01_Play/PlayEnum.h>
#include <Mode/01_Play/PlayPlayerState.h>
#include <Mode/01_Play/PlayGameMode.h>

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

USTRUCT(BlueprintType)
struct FCurLevelInfo_GAMESTATE
{
	GENERATED_BODY()

	FCurLevelInfo_GAMESTATE() {}
	FCurLevelInfo_GAMESTATE(const FCurLevelInfo_GAMEMODE& _InsInfo)
	{
		LevelAssetName = _InsInfo.LevelAssetName;
		LevelName = _InsInfo.LevelName;
		LevelType = _InsInfo.LevelType;
		EndCondition = _InsInfo.EndCondition;
		StageLimitTime = _InsInfo.StageLimitTime;
		PlayGuide = _InsInfo.PlayGuide;
		GoalGuide = _InsInfo.GoalGuide;
		LevelIMG = _InsInfo.LevelIMG;
		LevelTagIMG = _InsInfo.LevelTagIMG;
		CurStagePhase = _InsInfo.CurStagePhase;
	}

	UPROPERTY()
	FString LevelAssetName = TEXT("");					// 레벨 에셋 이름
	UPROPERTY()
	FString LevelName = TEXT("");						// 레벨 이름
	UPROPERTY()
	EStageType LevelType = EStageType::NONE;			// 레벨 타입
	UPROPERTY()
	EPlayerStatus EndCondition = EPlayerStatus::NONE;	// 레벨 종료 조건
	UPROPERTY()
	float StageLimitTime = 0.0f;						// 레벨 제한 시간
	UPROPERTY()
	FString PlayGuide = TEXT("");						// 레벨 플레이 가이드
	UPROPERTY()
	FString GoalGuide = TEXT("");						// 레벨 목표 가이드
	UPROPERTY()
	UTexture2D* LevelIMG = nullptr;						// 레벨 이미지
	UPROPERTY()
	UTexture2D* LevelTagIMG = nullptr;					// 레벨 태그 이미지
	UPROPERTY()
	EStagePhase CurStagePhase = EStagePhase::STAGE_1;	// 현재 스테이지 단계
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

	// 위젯 델리게이트 맵
	TMap<FName, FWidgetDelegate> WidgetDelegates;

	// 델리게이트 등록 함수
	void RegisterWidgetDelegate(FName _Name, FWidgetDelegate InDelegate);

	UFUNCTION(NetMulticast, Reliable)
	void MCAST_WidgetDelegate(FName _Name);

#pragma region PlayGameState :: 플레이어 관련
public:
	// 전체 플레이어 리스트 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER LIST")
	TArray<FPlayerInfoEntry> GetPlayerInfoArray() { return PlayerInfoArray; }

	// 실패한 플레이어 리스트 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER LIST")
	TArray<FPlayerInfoEntry> GetFailPlayerInfoArray() { return FailPlayerInfoArray; }

	// 디폴트 플레이어 리스트 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER LIST")
	TArray<FPlayerInfoEntry> GetDefaultPlayerInfoArray() { return DefaultPlayerInfoArray; }

	// `PlayerState`에서 데이터 동기화
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PLAYER LIST")
	void SyncPlayerInfoFromPlayerState();
	void SyncPlayerInfoFromPlayerState_Implementation();

	// 접속자 수 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYERS")
	void SetConnectedPlayers(int _Count);
	void SetConnectedPlayers_Implementation(int _Count);

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

	// 실패한 플레이어 정보 리스트 백업
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PLAYERS")
	void BackUpFailPlayersInfo();
	void BackUpFailPlayersInfo_Implementation();

	// 실패한 유저의 떨어지는 순번을 정해줌
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PLAYERS")
	void SetDropOrder();
	void SetDropOrder_Implementation();

	UFUNCTION(BlueprintCallable, Category = "PLAYERS")
	void RestoreFailPlayersInfo();


protected:
	// 전체 플레이어 정보 목록
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "PLAYER LIST")
	TArray<FPlayerInfoEntry> PlayerInfoArray;

	// 플레이어 정보 캐싱용 맵 (UID → PlayerInfo)
	TMap<FString, FPlayerInfo> CachedPlayerInfoMap;

	// 실패한 플레이어 정보 목록
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "PLAYER LIST")
	TArray<FPlayerInfoEntry> FailPlayerInfoArray;

	// 성공한 플레이어 정보 목록
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "PLAYER LIST")
	TArray<FPlayerInfoEntry> DefaultPlayerInfoArray;

	// 접속한 플레이어의 수
	UPROPERTY(Replicated)
	int ConnectedPlayers = 0;

#pragma endregion

#pragma region PlayGameState :: 레벨 데이터 관련
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "LEVEL")
	FCurLevelInfo_GAMESTATE CurLevelInfo_GameState;

// 레벨 데이터들 얻는 함수
public:
	// 레벨 에셋 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetLevelAssetName_STATE() const { return CurLevelInfo_GameState.LevelAssetName; }

	// 레벨 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetLevelName_STATE() const { return CurLevelInfo_GameState.LevelName; }

	// 레벨 타입 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	EStageType GetLevelType_STATE() const { return CurLevelInfo_GameState.LevelType; }

	// 레벨 제한 시간 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	float GetStageLimitTime_STATE() const { return CurLevelInfo_GameState.StageLimitTime; }

	// 레벨 플레이 가이드 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetPlayGuide_STATE() const { return CurLevelInfo_GameState.PlayGuide; }

	// 레벨 목표 가이드 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetGoalGuide_STATE() const { return CurLevelInfo_GameState.GoalGuide; }

	// 레벨 이미지 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	UTexture2D* GetLevelIMG_STATE() const { return CurLevelInfo_GameState.LevelIMG; }

	// 레벨 태그 이미지 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	UTexture2D* GetLevelTagIMG_STATE() const { return CurLevelInfo_GameState.LevelTagIMG; }

	// 레벨 페이즈 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	EStagePhase GetCurStagePhase_STATE() const { return CurLevelInfo_GameState.CurStagePhase; }

	// 레벨의 골 타입을 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	EPlayerStatus GetEndCondition_STATE() const { return CurLevelInfo_GameState.EndCondition; }
	
	// 현 스테이지의 골 타입을 변환하고 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString GetSTATEStageGoalType();

// 시작 및 종료를 위한 데이터
public:
	// 레벨 시네마틱 시작해도 되나요?
	bool GetCanStartLevelCinematic() { return CanStartLevelCinematic; }

	// 레벨 시네마틱 끝났나요?
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	bool GetIsLevelCinematicEnd() { return IsLevelCinematicEnd; }

	// 골인 목표 인원 수 세팅 됐는지 확인할게
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	bool GetGameStateSettedGoalCount() { return bGameStateSettedGoalCount; }

	// 골인 목표 인원 수 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	int GetGameStateFinishPlayer() { return GameStateFinishPlayer; }

	// 현재 골인한 플레이어 수 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	int GetGameStateCurFinishPlayer() { return GameStateCurFinishPlayer; }

	// 결과 화면인지 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	bool GetGameStateIsResultLevel() { return bGameStateIsResultLevel; }

	// 게임 시작했니?
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	bool GetGameStateGameStarted() { return bGameStateGameStarted; }

	// 결과 화면에서 다음 스테이지로 넘어가도 되니?
	bool GetCanMoveResultLevel() { return bCanMoveResultLevel; }

	// 최대 접속자 수를 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	int GetStateMaxPlayerCount() const { return StateMaxPlayerCount; }

	// 게임 끝났니?
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	bool GetStateIsEndGame() const { return StateIsEndGame; }

// 레벨 데이터들 세팅하는 함수
public:
	// 레벨 시네마틱 시작하세요
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetCanStartLevelCinematic();
	void SetCanStartLevelCinematic_Implementation();

	// 레벨 시네마틱 끝났어요
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetIsLevelCinematicEnd(bool _Value);
	void SetIsLevelCinematicEnd_Implementation(bool _Value);

	// 골인 목표 인원 수 세팅 완료 했는지
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateGoalCountSetted(bool _Value);
	void SetGameStateGoalCountSetted_Implementation(bool _Value);

	// 골인 목표 인원 수 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateFinishPlayer(int _Value);
	void SetGameStateFinishPlayer_Implementation(int _Value);
	
	// 골인한 플레이어 수 세팅 : PlaytGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateCurFinishPlayer(int _Count);
	void SetGameStateCurFinishPlayer_Implementation(int _Count);

	// 결과 화면인지 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateIsResultLevel(bool _Value);
	void SetGameStateIsResultLevel_Implementation(bool _Value);

	// 게임 시작했음을 세팅 : PlayGameMode에서 호출
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetGameStateGameStarted(bool _Value);
	void SetGameStateGameStarted_Implementation(bool _Value);

	// 다음 레벨로 이동 가능해요 : PlayGameMode에 세팅
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	void STATESetCanMoveLevel(bool _b);

	// 결과 화면에서 다음 스테이지로 넘어가도록 해 + PlayGameMode에 세팅
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetCanMoveResultLevelTrue();
	void SetCanMoveResultLevelTrue_Implementation();
	
	// 최대 접속자 수 세팅 : PlayGameMode에서 세팅
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetStateMaxPlayerCount(int _Value);
	void SetStateMaxPlayerCount_Implementation(int _Value);

	// 게임 끝났음 : PlayGameMode에서 세팅
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "LEVEL")
	void SetStateIsEndGameTrue();
	void SetStateIsEndGameTrue_Implementation();

protected:
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
	// 현재 골인한 플레이어 수
	UPROPERTY(Replicated)
	int GameStateCurFinishPlayer = 0;
	// 결과 화면이니?
	UPROPERTY(Replicated)
	bool bGameStateIsResultLevel = false;
	// 게임 시작했니?
	UPROPERTY(Replicated)
	bool bGameStateGameStarted = false;
	// 결과 화면에서 넘어가도 될까?
	UPROPERTY(Replicated)
	bool bCanMoveResultLevel = false;
	// 최대 접속자 수 : PlayGameMode에서 가져옴
	UPROPERTY(Replicated)
	int StateMaxPlayerCount = 0;
	// 게임 끝났음
	UPROPERTY(Replicated)
	bool StateIsEndGame = false;

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

	UPROPERTY(Replicated)
	bool STATECanMoveToResultLevel = false;

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