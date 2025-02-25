// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API ATitleGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	void Tick(float _Delta) override;

public:
	ATitleGameMode();

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
