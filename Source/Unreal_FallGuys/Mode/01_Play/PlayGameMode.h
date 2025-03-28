// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include <Global/GlobalEnum.h>

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

	// 플레이어 접속시 실행되는 함수
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 인원 충족 했는지 체크
	UFUNCTION(BlueprintCallable, Category = "PLAY GAME")
	void CheckNumberOfPlayer(class APlayGameState* _PlayState);

	// 게임 시작
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "PLAY GAME")
	void StartGame();
	void StartGame_Implementation();

	// 플레이어 정보 동기화
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "PLAY GAME")
	void SyncPlayerInfo();
	void SyncPlayerInfo_Implementation();

	// 캐릭터 이동 가능하게 세팅
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "PLAY GAME")
	void SetCharacterMovePossible();
	void SetCharacterMovePossible_Implementation();

	// 게임 시작 전 카운트다운 핸들 활성화
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void StartCountdownTimer();
	void StartCountdownTimer_Implementation();

	// 스테이지 제한 시간 타이머 활성화
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void StartStageLimitTimer();
	void StartStageLimitTimer_Implementation();

	// 스테이지 제한 시간 오버 처리
	void OnStageLimitTimeOver();

protected:
	virtual void Tick(float DeltaSeconds) override;
	void BeginPlay() override;
	
	// 게임 시작 전 카운트 다운 핸들
	FTimerHandle CountdownTimerHandle;

	// 스테이지 제한 시간 핸들
	FTimerHandle StageLimitTimerHandle;

	// 카운트다운 시작 (3초 대기 후 실행)
	void StartCountdown();

	// 카운트다운 진행 (매초 실행)
	void UpdateCountdown();

	// 동기화 변수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// 접속 제한
	bool InvalidConnect = false;
	// 인원 충족
	bool pNumberOfPlayer = false;
	// 게임 시작 카운트다운 끝
	bool pCountDownEnd = false;
	// 캐릭터 이동
	bool pPlayerMoving = false;

//LMH
private:
	// 골인 목표 인원 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	int FinishPlayer = 2;

	// 골인한 플레이어 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	int CurFinishPlayer = 0;

	bool IsEndGame = false;

	// 다음 스테이지 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString NextLevel= TEXT("PlayLevel");

public:
	UFUNCTION(BlueprintCallable)
	void ServerTravelToNextMap(const FString& url);

	UFUNCTION(BlueprintCallable)
	void OnPlayerFinished()
	{
		CurFinishPlayer += 1;
	}

	UFUNCTION(BlueprintCallable)
	void SetFinishPlayerCount(int _p)
	{
		FinishPlayer = _p;
	}
};
