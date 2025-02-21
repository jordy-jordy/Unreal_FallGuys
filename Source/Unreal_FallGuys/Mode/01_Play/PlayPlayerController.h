// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API APlayPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;

};
