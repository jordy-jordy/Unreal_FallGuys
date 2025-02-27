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

	/** 현재 접속한 플레이어 수 */
	UPROPERTY(ReplicatedUsing = OnRep_ConnectedPlayers)
	int32 ConnectedPlayers;

	/** 플레이어 수 변경 시 클라이언트에서 실행되는 함수 */
	UFUNCTION()
	void OnRep_ConnectedPlayers();

	/** 플레이어 접속 시 실행되는 함수 */
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
