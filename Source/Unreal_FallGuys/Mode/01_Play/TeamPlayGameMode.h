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

	// PlayGameMode의 PostLogin 다음으로 실행
	virtual void PostLogin(APlayerController* _NewPlayer) override;

	// 레드팀 점수 계산
	UFUNCTION(BlueprintCallable, Category = "SCORE")
	void CountREDTeamScore(int _NumberOfEgg);

	// 블루팀 점수 계산
	UFUNCTION(BlueprintCallable, Category = "SCORE")
	void CountBLUETeamScore(int _NumberOfEgg);

protected:
	virtual void Tick(float DeltaSeconds) override;
	void BeginPlay() override;

	// 팀 배정
	void AssignTeam(class APlayPlayerState* _PlayerState);

	// 각 팀 점수
	UPROPERTY(BlueprintReadOnly)
	int32 REDTeamScore = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 BLUETeamScore = 0;

	// 팀전 제한 시간
	float MODE_StageLimitTime = 0.0f;
	// 제한 시간 타이머 활성화 됐니?
	bool IsStartedLimitTimer = false;

	// 스테이지 제한 시간 타이머 활성화
	UFUNCTION(BlueprintCallable, Category = "LEVEL LIMIT TIME")
	void StartStageLimitTimer();

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
