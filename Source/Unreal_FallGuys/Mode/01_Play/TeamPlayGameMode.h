// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/PlayGameMode.h"
#include "TeamPlayGameMode.generated.h"


/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API ATeamPlayGameMode : public APlayGameMode
{
	GENERATED_BODY()

public:
	ATeamPlayGameMode();

	// 각 팀 점수 계산
	UFUNCTION(BlueprintCallable, Category = "TEAM SCORE") // 레드
	void CountREDTeamScore(int _NumberOfEgg);
	UFUNCTION(BlueprintCallable, Category = "TEAM SCORE") // 블루
	void CountBLUETeamScore(int _NumberOfEgg);
	
	// 각 팀 점수 반환
	UFUNCTION(BlueprintCallable, Category = "TEAM SCORE") // 레드
	int32 GetREDTeamScore() { return REDTeamScore; }
	UFUNCTION(BlueprintCallable, Category = "TEAM SCORE") // 블루
	int32 GetBLUETeamScore() { return BLUETeamScore; }

	// 점수에 따라 승리팀, 패배팀 구분
	void DetermineWinningAndLosingTeams();

protected:
	// PlayGameMode의 틱, 비긴플레이, 포스트로긴 다음에 실행됨
	virtual void Tick(float DeltaSeconds) override;
	void BeginPlay() override;
	virtual void PostLogin(APlayerController* _NewPlayer) override;
	// 심리스 서버 트래블 이후 플레이어 처리
	virtual void HandleSeamlessTravelPlayer(AController*& _NewController) override;

	// 각 팀 점수
	UPROPERTY(BlueprintReadOnly)
	int32 REDTeamScore = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 BLUETeamScore = 0;
	// 팀전 제한 시간
	float MODE_StageLimitTime = 0.0f;
	// 제한 시간 타이머 활성화 됐니?
	bool bStartedLimitTimer = false;
	// 승패 결정 됐어?
	bool bSettedWINLOSE = false;

	// 게임 시작 후 스테이지 제한 시간 핸들
	FTimerHandle StageLimitTimerHandle;
	// 게임 시작 후 남은 제한 시간 핸들
	FTimerHandle RemainingTimeUpdateHandle;
	// 서버 트래블 딜레이 핸들
	FTimerHandle TravelDelayTimerHandle;

	// 플레이어 팀 배정
	void AssignTeam(class APlayPlayerState* _PlayerState);

	// 팀전 종료 로직
	void SetEndCondition_Team();
	// 팀전 다음 레벨의 정보 세팅
	void SetNextTeamLevelData();
	// 팀전용 : 플레이어 정보 백업
	void BackUpTeamPlayersInfo();

	// 다음 팀전 맵으로 이동
	UFUNCTION(BlueprintCallable)
	void ServerTravelToNextTeamMap();

	// 로비로 돌아가
	UFUNCTION(BlueprintCallable)
	void ClientTravelToTitle();

	// 스테이지 제한 시간 타이머 활성화
	UFUNCTION(BlueprintCallable, Category = "LEVEL LIMIT TIME")
	void StartStageLimitTimer();

	// 스테이지 남은 제한 시간 타이머 활성화
	UFUNCTION(BlueprintCallable, Category = "LEVEL LIMIT TIME")
	void UpdateRemainingTime();

	// 스테이지 제한 시간 오버 처리
	UFUNCTION(BlueprintCallable, Category = "LEVEL LIMIT TIME")
	void OnStageLimitTimeOver();


//LMH
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Team")
	TMap<ETeamType, int> TeamFloors;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void CalTeam();

	UFUNCTION(BlueprintCallable)
	TMap<ETeamType, int> GetTeamFloors();
};
