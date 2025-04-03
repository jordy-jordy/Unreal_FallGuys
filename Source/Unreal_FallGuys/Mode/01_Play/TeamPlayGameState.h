// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/PlayGameState.h"
#include "TeamPlayGameState.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API ATeamPlayGameState : public APlayGameState
{
	GENERATED_BODY()

public:
	// 레드팀 점수 : TeamPlayGameMode에서 호출됨
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "SCORE")
	void SetREDTeamScore(int32 _Score);
	void SetREDTeamScore_Implementation(int32 _Score);

	// 블루팀 점수 : TeamPlayGameMode에서 호출됨
	UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "SCORE")
	void SetBLUETeamScore(int32 _NumberOfEgg);
	void SetBLUETeamScore_Implementation(int32 _Score);

	// 레드팀 점수 반환
	UFUNCTION(BlueprintCallable, Category = "SCORE")
	int32 GetGameStateREDTeamScore() { return GameStateREDTeamScore; }
	// 블루팀 점수 반환
	UFUNCTION(BlueprintCallable, Category = "SCORE")
	int32 GetGameStateBLUETeamScore() { return GameStateBLUETeamScore; }

protected:
	// 각 팀 점수 : TeamPlayGameMode에서 지정해줌
	UPROPERTY(Replicated)
	int32 GameStateREDTeamScore = 0;
	UPROPERTY(Replicated)
	int32 GameStateBLUETeamScore = 0;

private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
