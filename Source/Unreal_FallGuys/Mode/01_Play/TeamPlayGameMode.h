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


protected:
	// 팀 배정
	void AssignTeam(class APlayPlayerState* _PlayerState);

	// 각 팀 점수
	UPROPERTY(BlueprintReadOnly)
	int32 REDTeamScore = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 BLUETeamScore = 0;

	// 각 팀 점수
	UPROPERTY(BlueprintReadOnly)
	int32 REDTeamEggCount = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 BLUETeamEggCount = 0;

	// 점수 증가
	//UFUNCTION(BlueprintCallable, Category = "SCORE")
	//void CountTeamScore(int32 _NumberOfEgg);

//LMH
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Team")
	TMap<ETeamType, int> TeamFloors;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void CalTeam();

	UFUNCTION(BlueprintCallable)
	TMap<ETeamType, int> GetTeamFloors();
};
