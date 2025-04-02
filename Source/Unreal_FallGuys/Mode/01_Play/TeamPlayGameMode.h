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
	// PlayGameMode의 PostLogin 다음으로 실행
	virtual void PostLogin(APlayerController* _NewPlayer) override;

	// 레드팀 점수 계산
	UFUNCTION(BlueprintCallable, Category = "SCORE")
	void CountREDTeamScore(int _NumberOfEgg);

	// 블루팀 점수 계산
	UFUNCTION(BlueprintCallable, Category = "SCORE")
	void CountBLUETeamScore(int _NumberOfEgg);

protected:
	// 팀 배정
	void AssignTeam(class APlayPlayerState* _PlayerState);

	// 각 팀 점수
	UPROPERTY(BlueprintReadOnly)
	int32 REDTeamScore = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 BLUETeamScore = 0;

	// 각 팀의 달걀 수
	UPROPERTY(BlueprintReadWrite)
	int REDTeamEggCount = 0;
	UPROPERTY(BlueprintReadWrite)
	int BLUETeamEggCount = 0;


//LMH
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Team")
	TMap<ETeamType, int> TeamFloors;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void CalTeam();

	UFUNCTION(BlueprintCallable)
	TMap<ETeamType, int> GetTeamFloors();
};
