// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "EndPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API AEndPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetupInputComponentEvent();

	const UInputAction* GetInputAction(FStringView _Name);

protected:
	void BeginPlay() override;
	void SetupInputComponent();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TMap<FString, const UInputAction*> MappingActions;
};
