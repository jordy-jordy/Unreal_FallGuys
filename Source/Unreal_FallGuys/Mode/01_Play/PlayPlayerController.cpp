// Fill out your copyright notice in the Description page of Project Settings.


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
		if (InputAction_LevelCinematicEnd)
		{
			EnhancedInput->BindAction(InputAction_LevelCinematicEnd, ETriggerEvent::Started, this, &APlayPlayerController::OnPrintLevelCinematicEnd);
		}
		if (InputAction_CurFinishPlayer)
		{
			EnhancedInput->BindAction(InputAction_CurFinishPlayer, ETriggerEvent::Started, this, &APlayPlayerController::OnPrintCurFinishPlayer);
		}
	}
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

// 디버그용 : 레벨 시네마틱 바로 끝나게
void APlayPlayerController::OnPrintLevelCinematicEnd()
{
	if (UBaseGameInstance* GameIns = GetGameInstance<UBaseGameInstance>())
	{
		GameIns->InsetLevelCinematicEnd();
	}
}

// 디버그용 : 골인한 인원 및 목표 골인 인원 출력
void APlayPlayerController::OnPrintCurFinishPlayer()
{
	if (UBaseGameInstance* GameIns = GetGameInstance<UBaseGameInstance>())
	{
		GameIns->InsGetGameStateCurFinishPlayer();
	}
}

// EndLevel로 이동
void APlayPlayerController::Client_TravelToEndLevel_Implementation()
{
	if (IsLocalController() && GetWorld())
	{
		ClientTravel("/Game/BP/Level/02_End/EndLevel", ETravelType::TRAVEL_Absolute);
	}
}
