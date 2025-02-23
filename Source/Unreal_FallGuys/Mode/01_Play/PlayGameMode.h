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
	
protected:
	void BeginPlay() override;
	void Tick(float _Delta) override;

};
