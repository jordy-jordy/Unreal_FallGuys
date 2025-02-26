// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

private:
	void SetupInputComponent();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext = nullptr;
};
