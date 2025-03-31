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

	// 플레이어가 완전히 준비된 이후 실행되는 함수
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* _NewPlayer) override;

protected:
	virtual void Tick(float DeltaSeconds) override;
	void BeginPlay() override;

#pragma endregion

#pragma region PlayGameMode :: 게임 시작 관련
public:
	// 게임 시작
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "PLAY GAME")
	void StartGame();
	void StartGame_Implementation();

	// 인원 충족 했는지 체크
	UFUNCTION(BlueprintCallable, Category = "PLAY GAME")
	void CheckNumberOfPlayer(class APlayGameState* _PlayState);

	// 플레이어 정보 동기화
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "PLAY GAME")
	void SyncPlayerInfo();
	void SyncPlayerInfo_Implementation();

	// 캐릭터 이동 가능하게 세팅
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "PLAY GAME")
	void SetCharacterMovePossible();
	void SetCharacterMovePossible_Implementation();

	// 목표 골인 인원 수 반환
	UFUNCTION(BlueprintCallable)
	int32 GetFinishPlayerCount() const { return FinishPlayer; }

protected:
	// 목표 골인 인원 수 제어
	void ControllFinishPlayer(APlayGameState* _PlayState);

	// 목표 골인 인원 수 세팅
	void SetFinishPlayer(int32 _PlayerCount);

	// 접속 제한
	bool InvalidConnect = false;
	// 인원 충족
	bool pNumberOfPlayer = false;
	// 캐릭터 이동
	bool pPlayerMoving = false;
	// 게임 시작 카운트다운 끝
	bool pCountDownEnd = false;

#pragma endregion

#pragma region PlayGameMode :: 타이머 관련
public:
	// 게임 시작 전 카운트다운 핸들 활성화
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "TIMER")
	void StartCountdownTimer();
	void StartCountdownTimer_Implementation();

	// 스테이지 제한 시간 타이머 활성화
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "TIMER")
	void StartStageLimitTimer();
	void StartStageLimitTimer_Implementation();

protected:
	// 카운트 다운 핸들
	FTimerHandle CountdownTimerHandle;

	// 스테이지 제한 시간 핸들
	FTimerHandle StageLimitTimerHandle;

	// 카운트다운 시작 (3초 대기 후 실행)
	void StartCountdown();

	// 카운트다운 진행 (매초 실행)
	void UpdateCountdown();

	// 스테이지 제한 시간 오버 처리
	void OnStageLimitTimeOver();

#pragma endregion

#pragma region PlayGameMode :: Jump Show Down 스테이지 관련
public:
	// 플랫폼 등록 함수
	void AddShowDownPlatform(class AShowDownPlatform* _Platform);

	// 랜덤 스테이지 활성화 함수
	UFUNCTION()
	class AShowDownPlatform* GetRandomPlatform();

protected:
	// 전체 플랫폼 리스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShowDown", meta = (AllowPrivateAccess = "true"))
	TArray<class AShowDownPlatform*> AllPlatforms;

#pragma endregion


//LMH
protected:
	// 골인 목표 인원 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	int FinishPlayer = 99;

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
