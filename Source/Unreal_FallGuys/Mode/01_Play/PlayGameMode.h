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

	// 최소 인원 체크
	UFUNCTION(BlueprintCallable, Category = "PLAY GAME")
	bool IsMinPlayersReached();

	// 게임 시작
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "PLAY GAME")
	void StartGame();
	void StartGame_Implementation();

	// 플레이어 정보 동기화
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "PLAY GAME")
	void SyncPlayerInfo();
	void SyncPlayerInfo_Implementation();

protected:
	virtual void Tick(float DeltaSeconds) override;
	void BeginPlay() override;

	// 동기화 변수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 접속한 플레이어의 수
	UPROPERTY(ReplicatedUsing = OnRep_ConnectedPlayers)
	int ConnectedPlayers;

	// 플레이어 수 변경 시 클라이언트에서 실행될 함수
	UFUNCTION()
void OnRep_ConnectedPlayers(); 

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
