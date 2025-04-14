// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EndGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API AEndGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;
	void Tick(float _Delta) override;

// 이현정 : 승자에 대한 정보
protected:
	FString WinnerNickName = TEXT("");
	FString WinnerCostume_TOP = TEXT("");
	FString WinnerCostume_BOT = TEXT("");
	FString WinnerCostume_COLOR = TEXT("");


};
