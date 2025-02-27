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

	/** ���� ������ �÷��̾� �� */
	UPROPERTY(ReplicatedUsing = OnRep_ConnectedPlayers)
	int32 ConnectedPlayers;

	/** �÷��̾� �� ���� �� Ŭ���̾�Ʈ���� ����Ǵ� �Լ� */
	UFUNCTION()
	void OnRep_ConnectedPlayers();

	/** �÷��̾� ���� �� ����Ǵ� �Լ� */
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
