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

#pragma region TeamPlayGameState :: 팀 관련
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

#pragma endregion

#pragma region TeamPlayGameState :: 스테이지 제한 시간 관련
public:
	// Stage 제한 시간 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL LIMIT TIME")
	float GetStageLimitTime() { return STATE_StageLimitTime; }

	// Stage 제한 시간 결정 함수
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "LEVEL LIMIT TIME")
	void SetStageLimitTime(float _Time);
	void SetStageLimitTime_Implementation(float _Time);

protected:
	// Stage 제한 시간 - 외부에서 값 가져옴
	UPROPERTY(Replicated)
	float STATE_StageLimitTime;

#pragma endregion

private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//LMHH
public:
	UFUNCTION(BlueprintCallable)
	void SpawnEggManager();

	UPROPERTY(EditAnywhere, Category = "Manager")
	TSubclassOf<class AEggSpawnManager> SpawnManagerFactory;

	UPROPERTY(BlueprintReadWrite, Category = "Manager")
	class AEggSpawnManager* Spawnner;

	UFUNCTION(BlueprintCallable)
	TArray<int> GetTeamEggCount() const;
};
