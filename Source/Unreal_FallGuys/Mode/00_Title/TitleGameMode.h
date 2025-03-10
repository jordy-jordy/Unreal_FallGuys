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

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level", meta = (AllowPrivateAccess = "true"))
	FString MapName = TEXT("");	
};
