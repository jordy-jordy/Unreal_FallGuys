// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "PlayPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API APlayPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void AddMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetupInputComponentEvent();

protected:
	void BeginPlay() override;
	void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext = nullptr;
};
