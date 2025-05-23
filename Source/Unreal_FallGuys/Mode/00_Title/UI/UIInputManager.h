// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "UIInputManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FALLGUYS_API UUIInputManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUIInputManager();

	void SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent);

	// Select
	void SwitchWidget_SPACE();
	void SwitchWidget_ESC();

	// Menu
	void SwitchWidget_Q();
	void SwitchWidget_E();

	// Move
	void SwitchWidget_W();
	void SwitchWidget_A();
	void SwitchWidget_S();
	void SwitchWidget_D();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
