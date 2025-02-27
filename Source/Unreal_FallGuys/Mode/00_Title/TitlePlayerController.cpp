// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitlePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


void ATitlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetupInputComponentEvent();
}

void ATitlePlayerController::AddMappingContext(UInputMappingContext* _MappingContext)
{
	if (nullptr == GetLocalPlayer())
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	InputSystem->ClearAllMappings();
	InputSystem->AddMappingContext(_MappingContext, 0);
}