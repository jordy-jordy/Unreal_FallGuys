﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Unreal_FallGuys.h"
#include "Global/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/01_Play/PlayGameMode.h"


void APlayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly Mode;
	SetInputMode(Mode);
}

void APlayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetupInputComponentEvent();

	// 이현정 : 디버그용 Input 설정
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (InputAction_PlayerInfo)
		{
			EnhancedInput->BindAction(InputAction_PlayerInfo, ETriggerEvent::Started, this, &APlayPlayerController::OnPrintPlayerInfo);
		}
		if (InputAction_ConnectedPlayers)
		{
			EnhancedInput->BindAction(InputAction_ConnectedPlayers, ETriggerEvent::Started, this, &APlayPlayerController::OnPrintConnectedPlayers);
		}
		if (InputAction_LevelName)
		{
			EnhancedInput->BindAction(InputAction_LevelName, ETriggerEvent::Started, this, &APlayPlayerController::OnPrintLevelName);
		}
	}

}

void APlayPlayerController::AddMappingContext(UInputMappingContext* _MappingContext)
{

	if (nullptr == GetLocalPlayer())
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	InputSystem->ClearAllMappings();
	InputSystem->AddMappingContext(_MappingContext, 0);
}

// 이현정
// 디버그용 : 플레이어 태그 확인
void APlayPlayerController::OnPrintPlayerInfo()
{
	if (UBaseGameInstance* GameIns = GetGameInstance<UBaseGameInstance>())
	{
		GameIns->InsPrintPlayerInfo();
	}
}

// 디버그용 : 접속자 수 확인
void APlayPlayerController::OnPrintConnectedPlayers()
{
	if (UBaseGameInstance* GameIns = GetGameInstance<UBaseGameInstance>())
	{
		GameIns->InsPrintConnectedPlayers();
	}
}

// 디버그용 : 레벨 이름 확인
void APlayPlayerController::OnPrintLevelName()
{
	if (UBaseGameInstance* GameIns = GetGameInstance<UBaseGameInstance>())
	{
		GameIns->InsPrintLevelName();
	}
}