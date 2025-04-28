// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include <Global/GlobalEnum.h>
#include <Global/BaseGameInstance.h>
#include <Mode/01_Play/PlayEnum.h>
#include <Mode/01_Play/PlayPlayerState.h>

#include "PlayGameMode.generated.h"


USTRUCT(BlueprintType)
struct FCurLevelInfo_GAMEMODE
{
	GENERATED_BODY()

	FCurLevelInfo_GAMEMODE() {}
	FCurLevelInfo_GAMEMODE(const FCurLevelInfo_GAMEINS& _InsInfo)
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
class UNREAL_FALLGUYS_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	APlayGameMode();

protected:
	FCurLevelInfo_GAMEMODE CurLevelInfo_Mode;

#pragma region PlayGameMode :: 핵심 함수
public: 
protected:
	// 플레이어 접속시 실행되는 함수 :: 가장 빠름
	virtual void PreLogin(
		const FString& _Options,
		const FString& _Address,
		const FUniqueNetIdRepl& _UniqueId,
		FString& _ErrorMessage
	) override;

	// 플레이어 최초 접속시 실행되는 함수 :: PreLogin 다음
	virtual void PostLogin(APlayerController* _NewPlayer) override;
	// 심리스 서버 트래블 이후 플레이어 처리
	virtual void HandleSeamlessTravelPlayer(AController*& _NewController) override;

	virtual void Tick(float DeltaSeconds) override;
	void BeginPlay() override;

#pragma endregion

#pragma region PlayGameMode :: 플레이어 정보 관련
public:
	// 플레이어 정보 동기화
	UFUNCTION(BlueprintCallable, Category = "PLAYGAMEMODE :: PLAYER")
	void SyncPlayerInfo();

	// 캐릭터 이동 가능하게 세팅
	UFUNCTION(BlueprintCallable, Category = "PLAYGAMEMODE :: PLAYER")
	void SetCharacterMovePossible();

	// 모든 DEFAULT 상태의 플레이어를 FAIL로 전환
	UFUNCTION(BlueprintCallable, Category = "PLAYGAMEMODE :: PLAYER")
	void SetDefaultPlayersToFail();

	// 모든 DEFAULT 상태의 플레이어를 SUCCESS로 전환
	UFUNCTION(BlueprintCallable, Category = "PLAYGAMEMODE :: PLAYER")
	void SetDefaultPlayersToSuccess();


#pragma endregion

#pragma region PlayGameMode :: 게임 시작 관련
public:
	// 게임 시작
	UFUNCTION(BlueprintCallable, Category = "PLAYGAMEMODE :: GAME")
	void StartGame();

	// 인원 충족 했는지 체크
	UFUNCTION(BlueprintCallable, Category = "PLAYGAMEMODE :: GAME")
	void CheckPlayersCount();

	// 접속 제한 세팅
	void SetPlayerAccessInvalid();

	// 목표 골인 인원 수 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYGAMEMODE :: GAME")
	int32 GetFinishPlayerCount() const { return FinishPlayer; }

	// 시네마틱 끝났대
	UFUNCTION(BlueprintCallable, Category = "PLAYGAMEMODE :: GAME")
	void SetCinematicEND(bool _Value) { bCinematicEND = _Value; }


protected:
	// 최초 접속시에 실행
	void HandleFirstTimeLogin(APlayerController* _NewPlayer, APlayPlayerState* _PlayerState, APlayGameState* _FallState, UBaseGameInstance* _GameInstance);

	// 필수 데이터 세팅
	bool SetupCommonEssentialData(class APlayerController* _NewPlayer, class APlayGameState*& _OutFallState, class APlayPlayerState*& _OutPlayerState, class UBaseGameInstance*& _OutGameInstance);
	// 플레이어 인원 플러스
	void AddPlayerCount(APlayGameState* _FallState);
	// 결과 화면인지 게임 인스로부터 가져옴
	void GetIsResultLevel(APlayPlayerState* _PlayerState, APlayGameState* _FallState, UBaseGameInstance* _GameInstance);
	// 레벨 이동 했는지 게임 인스로부터 가져옴
	bool GetIsLevelMoved(UBaseGameInstance* _GameInstance);
	// 플레이어 태그 생성
	FName GenerateUniquePlayerTag(APlayerController* _NewPlayer, int32 _PlayerIndex);
	// 세로운 플레이어 정보 세팅
	void InitPlayerInfo(class APlayerController* _NewPlayer, class APlayPlayerState* _PlayerState, class APlayGameState* _FallState, class UBaseGameInstance* _GameInstance);
	// 기존 플레이어 정보 복구
	void RestorePlayerInfo(class APlayerController* _NewPlayer, class APlayPlayerState* _PlayerState, class APlayGameState* _FallState, class UBaseGameInstance* _GameInstance);
	// 플레이어 등록이 완료 된 후 실행되는 공통로직
	void PostInitializePlayer(APlayGameState* _FallState);
	// 플레이어 인포 로그
	void LogPlayerInfo(const FString& _Prefix, const FPlayerInfo& _Info, APlayerController* _Controller);
	// 시네마틱 호출
	void StartCinematicIfReady(APlayGameState* _FallState);
	// 레벨 시네마틱 시작을 호출
	UFUNCTION()
	void CallLevelCinematicStart(APlayGameState* _PlayState);

	// 상태가 디폴트인 플레이어 수
	int32 GetDefaultPlayerCount();

	// 목표 골인 인원 수 제어
	void ControllFinishPlayer();

	// 목표 골인 인원 수 : 레이싱
	void FinishPlayer_Race();

	// 목표 골인 인원 수 : 생존
	void FinishPlayer_Survive();

	// 게임 시작 조건 검사 함수
	void CheckStartConditions();

	// 레벨 이동했니
	bool bMODEIsLevelMoved = false;
	// 접속 제한
	bool bInvalidConnect = false;
	// 인원 충족
	bool bNumberOfPlayer = false;
	// 목표 인원 수 세팅했니
	bool bSettedGoalCount = false;
	// 캐릭터 이동
	bool bPlayerMoving = false;
	// 게임 시작 카운트다운 썼니
	bool bCountDownStarted = false;	
	// 게임 시작 카운트다운 끝
	bool bCountDownEnd = false;
	// 결과 화면이니
	bool bMODEIsResultLevel = false;
	// 게임 시작됐니
	bool bGameStarted = false;
	// 시네마틱 끝났니
	bool bCinematicEND = false;

	// 디폴트 상태의 플레이어 수
	int32 DefaultPlayerCount = 0;

#pragma endregion

#pragma region PlayGameMode :: 타이머 관련
public:
	// 게임 시작 전 카운트다운 핸들 활성화
	UFUNCTION(BlueprintCallable, Category = "TIMER")
	void StartCountdownTimer();

protected:
	// 게임 시작 조건 검사 타이머 핸들
	FTimerHandle GameStartConditionTimer;
	// 레벨 시네마틱을 시작하도록 하는 타이머
	FTimerHandle SetLevelCinematicStartTimer;
	// 게임 시작 전 카운트 다운 핸들
	FTimerHandle CountdownTimerHandle;
	// 동기화 타이머 핸들
	FTimerHandle SyncPlayerInfoTimer;
	// 결과 화면 타이머 핸들
	FTimerHandle ResultTravelTimerHandle;

	// 카운트다운 시작 (3초 대기 후 실행)
	void StartCountdown();

	// 카운트다운 진행 (매초 실행)
	void UpdateCountdown();

#pragma endregion

#pragma region PlayGameMode :: 게임 종료 관련
public:
	// 레벨 이동 해도 되~
	UFUNCTION(Category = "PLAYGAMEMODE :: GAME")
	void SetCanMoveLevel(bool _Value)
	{
		bCanMoveLevel = _Value;
	}

	// 결과 화면에서 이동 해도 되~
	UFUNCTION(Category = "PLAYGAMEMODE :: GAME")
	void SetCanMoveResultLevel(bool _Value)
	{
		bCanMoveResultLevel = _Value;
	}

protected:
	// 게임 종료 트리거
	void SetEndCondition_Trigger(APlayGameState* _FallState);
	// 개인전 및 팀전 공통 종료 로직
	void SetEndCondition_Common(APlayGameState* _FallState);
	// 개인전 종료 로직
	void SetEndCondition_Solo(APlayGameState* _FallState);

	// 남은 플레이어의 상태 일괄 변경
	void ChangeDefaultPlayersTo();
	// 플레이어 정보 백업
	void BackUpPlayersInfo();
	// 개인전 다음 레벨의 정보 세팅
	void SetNextSoloLevelData();
	// 캐릭터 이동 불가능하게 세팅
	void SetCharacterMoveImPossible();
	// 최종 승리 플레이어를 마킹
	void MarkWinnersBeforeEndLevel();
	// 서버 트래블 할 준비 됐는지 체크
	bool AreAllClientsReady();

	// 동기화 타이머 해제 됐니
	bool bSyncCleared = false;
	// 레벨 종료 UI 띄웠니
	bool bShowedLevelEndUI = false;
	// 플레이어 상태 바꼈니
	bool bPlayerStatusChanged = false;
	// 플레이어 정보 백업했니
	bool bPlayerInfosBackUp = false;
	// 다음 레벨 세팅 됐니
	bool bNextLevelDataSetted = false;
	// 다음 레벨로 넘어가도 되니?
	bool bCanMoveLevel = false;
	// 서버 트래블 활성화 됐니?
	bool StartedServerTravel = false;
	// 부전승 처리 했니?
	bool bSetWinbyDefault = false;
	// 결과 화면에서 다음으로 넘어가도 돼?
	bool bCanMoveResultLevel = false;
	// 결과 화면에서 다음으로 넘어감
	bool bPassedResultLevel = false;
	// 플레이어들 모두 준비 됐니?
	bool bIsAllPlayersReady = false;

#pragma endregion

protected:
	// 최대 접속자 수
	int MaxPlayerCount = 0;

	// 대기중인 플레이어 수
	int32 WaitingPlayerCount = 0;	

	// 접속한 플레이어 수 (확정)
	int ConnectedPlayers = 0;

public:
	// 게임 끝났는지 확인
	bool GetIsEndGame() const { return IsEndGame; }

//LMH
protected:
	// 골인 목표 인원 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	int FinishPlayer = 99;

	// 현재 골인한 플레이어 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	int CurFinishPlayer = 0;

	bool IsEndGame = false;

	// 다음 스테이지 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString NextLevel= TEXT("LMHRaceOverMap");

	// End 레벨 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString EndLevel = TEXT("EndLevel");

public:
	// 개인전용 : 중간 결과창으로 이동
	UFUNCTION(BlueprintCallable)
	void ServerTravelToRaceOver();

	// 개인전용 : 다음 스테이지로 이동
	UFUNCTION(BlueprintCallable)
	void ServerTravelToNextRandLevel();

	// 개인전용 : 클라이언트 최종 결과창으로 이동 - 클라이언트
	UFUNCTION(BlueprintCallable)
	void ClientTravelToEndLevel();
	// 개인전용 : 클라이언트 최종 결과창으로 이동 - 서버
	UFUNCTION(BlueprintCallable)
	void ServerTravelToEndLevel();

	// 이현정 : 25.04.02 : 동기화 함수로 수정 : 골인 인원 +1 카운팅
	UFUNCTION(BlueprintCallable)
	void OnPlayerFinished(class APlayCharacter* _Character);

	// 이현정 : 25.04.02 : 동기화 함수로 수정 : 골인 목표 인원 수 세팅
	UFUNCTION(BlueprintCallable)
	void SetFinishPlayerCount(int _p);

	void SetSpectar_STAGE();
	void SetSpectar_RESULT();

	FTimerHandle SpectatorCheckTimerHandle;

	// 특정 클라이언트의 뷰 타겟을 랜덤 AlivePlayer로 설정
	void SetRandomViewForClient(class APlayerController* _TargetController);

	// 특정 클라이언트의 뷰 타겟을 인덱스 기반으로 설정
	void SetViewForClientByIndex(class APlayerController* _TargetController, int32 _TargetIndex);
};
