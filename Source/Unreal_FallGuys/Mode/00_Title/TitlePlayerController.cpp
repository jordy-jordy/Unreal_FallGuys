// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitlePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI Mode;
	SetInputMode(Mode);
	SetShowMouseCursor(true);
}

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

	TArray<FEnhancedActionKeyMapping> Map = _MappingContext->GetMappings();

	for (FEnhancedActionKeyMapping& Action : Map)
	{
		FString Name = Action.Action->GetName();

		MappingActions.Add(Name, Action.Action);
	}

	InputSystem->ClearAllMappings();
	InputSystem->AddMappingContext(_MappingContext, 0);
}

const UInputAction* ATitlePlayerController::GetInputAction(FStringView _Name)
{
	if (false == MappingActions.Contains(_Name.GetData()))
	{
		return nullptr;
	}

	return MappingActions[_Name.GetData()];
}