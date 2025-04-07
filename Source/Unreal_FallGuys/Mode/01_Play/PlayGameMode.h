// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include <Global/GlobalEnum.h>
#include <Mode/01_Play/PlayEnum.h>

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

#pragma region PlayGameMode :: 핵심 함수
public: 
	// 플레이어 접속시 실행되는 함수 :: 가장 빠름
	virtual void PreLogin(
		const FString& _Options,
		const FString& _Address,
		const FUniqueNetIdRepl& _UniqueId,
		FString& _ErrorMessage
	) override;

	// 플레이어 접속시 실행되는 함수 :: PreLogin 다음
	virtual void PostLogin(APlayerController* _NewPlayer) override;

protected:
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
	// 목표 골인 인원 수 제어
	void ControllFinishPlayer();

	// 게임 시작 조건 검사 함수
	void CheckStartConditions();

	// 레벨 시네마틱 시작을 호출
	UFUNCTION()
	void CallLevelCinematicStart(APlayGameState* _PlayState);

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

	// 현재 스테이지 이름
	FString MODE_CurLevelName = TEXT("Unknown");
	// 현재 스테이지 에셋 이름
	FString MODE_CurLevelAssetName = TEXT("Unknown");
	// 현재 스테이지 타입
	EStageType MODE_CurStageType = EStageType::NONE;
	// 현재 스테이지 단계
	EStagePhase MODE_CurStagePhase = EStagePhase::STAGE_1;
	// 스테이지 종료 기준 상태
	EPlayerStatus MODE_CurStageResultStatus = EPlayerStatus::NONE;
#pragma endregion

#pragma region PlayGameMode :: 타이머 관련
public:
	// 게임 시작 전 카운트다운 핸들 활성화
	UFUNCTION(BlueprintCallable, Category = "TIMER")
	void StartCountdownTimer();

	// 스테이지 제한 시간 타이머 활성화
	UFUNCTION(BlueprintCallable, Category = "TIMER")
	void StartStageLimitTimer();

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

	// 스테이지 제한 시간 오버 처리
	void OnStageLimitTimeOver();

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
	UFUNCTION(BlueprintCallable)
	void ServerTravelToNextMap(const FString& url);

	// 이현정 : 25.04.02 : 동기화 함수로 수정 : 골인 인원 +1 카운팅
	UFUNCTION(BlueprintCallable)
	void OnPlayerFinished();

	// 이현정 : 25.04.02 : 동기화 함수로 수정 : 골인 목표 인원 수 세팅
	UFUNCTION(BlueprintCallable)
	void SetFinishPlayerCount(int _p);



};
