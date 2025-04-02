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
	virtual void PostLogin(APlayerController* _NewPlayer) override;


private:
	void AssignTeam(class APlayPlayerState* _PlayerState);

//LMH
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Team")
	TMap<ETeamType, int> TeamFloors;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void CalTeam();

	UFUNCTION(BlueprintCallable)
	TMap<ETeamType, int> GetTeamFloors();
};
