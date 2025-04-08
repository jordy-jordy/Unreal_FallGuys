// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include <Global/GlobalEnum.h>
#include <Mode/01_Play/PlayEnum.h>

// 델리게이트 테스트
#include <Global/FallGlobal.h>

#include "PlayGameMode.generated.h"


/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	APlayGameMode();

	// 델리게이트 테스트
	void RegisterWidgetDelegate(FGameOverWidgetDelegate InDelegate);

	void GameOverWidgetDelegate();

	FGameOverWidgetDelegate WidgetDelegate;
	// 델리게이트 테스트

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

	// 플레이어 접속시 실행되는 함수 :: PreLogin 다음
	virtual void PostLogin(APlayerController* _NewPlayer) override;

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
	void CheckNumberOfPlayer(class APlayGameState* _PlayState);

	// 목표 골인 인원 수 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYGAMEMODE :: GAME")
	int32 GetFinishPlayerCount() const { return FinishPlayer; }

protected:
	// 레벨 시네마틱 시작을 호출
	UFUNCTION()
	void CallLevelCinematicStart(APlayGameState* _PlayState);

	// 목표 골인 인원 수 제어
	void ControllFinishPlayer();

	// 게임 시작 조건 검사 함수
	void CheckStartConditions();

	// 접속 제한
	bool InvalidConnect = false;
	// 인원 충족
	bool pNumberOfPlayer = false;
	// 캐릭터 이동
	bool pPlayerMoving = false;
	// 게임 시작 카운트다운 썼니
	bool pCountDownStarted = false;	
	// 게임 시작 카운트다운 끝
	bool pCountDownEnd = false;
	// 결과 화면이니
	bool bMODEIsResultLevel = false;
	// 게임 시작됐니
	bool bGameStarted = false;

	// 현재 스테이지 이름
	FString MODE_CurLevelName = TEXT("Unknown");
	// 현재 스테이지 에셋 이름
	FString MODE_CurLevelAssetName = TEXT("Unknown");
	// 현재 스테이지 타입
	EStageType MODE_CurStageType = EStageType::NONE;
	// 현재 스테이지 단계
	EStagePhase MODE_CurStagePhase = EStagePhase::STAGE_1;

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

	// 게임 시작 후 스테이지 제한 시간 핸들
	FTimerHandle StageLimitTimerHandle;

	// 동기화 타이머 핸들
	FTimerHandle SyncPlayerInfoTimer;

	// 카운트다운 시작 (3초 대기 후 실행)
	void StartCountdown();

	// 카운트다운 진행 (매초 실행)
	void UpdateCountdown();

#pragma endregion

#pragma region PlayGameMode :: 게임 종료 관련
public:
	// 레벨 이동 해도 되~
	UFUNCTION(Category = "PLAYGAMEMODE :: GAME")
	void SetCanMoveLevel(bool _Value) { bCanMoveLevel = _Value; }

protected:
	// 남은 플레이어의 상태 일괄 변경
	void ChangeDefaultPlayersTo();
	
	// 플레이어 정보 백업
	void BackUpPlayersInfo();

	// 개인전 다음 레벨의 정보 세팅
	void SetNextSoloLevelData();

	// 캐릭터 이동 불가능하게 세팅
	void SetCharacterMoveImPossible();

	// 게임 종료 트리거
	void SetEndCondition_Trigger();

	// 개인전 및 팀전 공용 종료 로직
	void SetEndCondition_Common();
	// 개인전 종료 로직
	void SetEndCondition_Solo();

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
	
	// 스테이지 종료 기준 상태
	EPlayerStatus MODE_CurStageResultStatus = EPlayerStatus::NONE;

#pragma endregion

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

public:
	// 개인전용 : 중간 결과창으로 이동
	UFUNCTION(BlueprintCallable)
	void ServerTravelToNextMap();

	// 이현정 : 25.04.02 : 동기화 함수로 수정 : 골인 인원 +1 카운팅
	UFUNCTION(BlueprintCallable)
	void OnPlayerFinished();

	// 이현정 : 25.04.02 : 동기화 함수로 수정 : 골인 목표 인원 수 세팅
	UFUNCTION(BlueprintCallable)
	void SetFinishPlayerCount(int _p);



};
