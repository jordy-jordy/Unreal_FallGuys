// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
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

	// 최소 인원 체크
	UFUNCTION(BlueprintCallable, Category = "GAME")
	bool IsMinPlayersReached();

	// 게임 시작
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "GAME")
	void StartGame();
	void StartGame_Implementation();

	// 플레이어 태그 설정
	UFUNCTION(BlueprintCallable, Category = "GAME")
	void AssignPlayerTag(APlayerController* _NewPlayer);

	// 플레이어 태그 동기화
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "GAME")
	void MulticastAssignPlayerTag(APlayerController* _NewPlayer, const FString& _Tag);
	void MulticastAssignPlayerTag_Implementation(APlayerController* _NewPlayer, const FString& _Tag);

protected:
	virtual void Tick(float DeltaSeconds) override;
	void BeginPlay() override;

	// 동기화 변수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 접속한 플레이어의 수
	UPROPERTY(ReplicatedUsing = OnRep_ConnectedPlayers)
	int ConnectedPlayers;

	// 현재 태그 숫자 카운트
	UPROPERTY(ReplicatedUsing = OnRep_PlayerCount)
	int PlayerCount = 0;

	// 플레이어 수가 변경되면 클라이언트에서 실행되는 함수
	UFUNCTION()
	void OnRep_ConnectedPlayers();

	// PlayerCount 변경 시 클라이언트에서 실행될 함수
	UFUNCTION()
	void OnRep_PlayerCount();

private:

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
