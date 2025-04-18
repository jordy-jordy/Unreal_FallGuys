﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Unreal_FallGuys.h"
#include "Global/BaseGameInstance.h"
#include "Mode/01_Play/PlayGameMode.h"
#include "Mode/01_Play/PlayPlayerState.h"


void APlayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	// 로컬 플레이어인 경우 비긴 플레이가 실행되고 얼마 후에 서버 트래블 준비가 되었음을 알림
	if (IsLocalController())
	{
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &APlayPlayerController::CallReady, 1.0f, false);
	}

	// 클라이언트에서 자기 GameInstance 정보 → 서버 PlayerState에 전달
	if (IsLocalController() && !HasAuthority())
	{
		UBaseGameInstance* ClientGameIns = Cast<UBaseGameInstance>(GetGameInstance());
		if (!ClientGameIns) return;
	
		FString NickName = ClientGameIns->InsGetNickname();
		FString CostumeTop = ClientGameIns->InsGetCostumeTop();
		FString CostumeBot = ClientGameIns->InsGetCostumeBot();
		FString CostumeColor = ClientGameIns->InsGetCostumeColor();

		Server_SetClientPlayerInfo(NickName, CostumeTop, CostumeBot, CostumeColor);
	}
	else if (IsLocalController() && HasAuthority())
	{
		UBaseGameInstance* ServerGameIns = Cast<UBaseGameInstance>(GetGameInstance());
		if (!ServerGameIns) return;
		APlayPlayerState* ServerState = GetPlayerState<APlayPlayerState>();
		if (!ServerState) return;

		FString NickName = ServerGameIns->InsGetNickname();
		FString CostumeTop = ServerGameIns->InsGetCostumeTop();
		FString CostumeBot = ServerGameIns->InsGetCostumeBot();
		FString CostumeColor = ServerGameIns->InsGetCostumeColor();

		ServerState->PlayerInfo.NickName = NickName;
		ServerState->PlayerInfo.CostumeTOP = CostumeTop;
		ServerState->PlayerInfo.CostumeBOT = CostumeBot;
		ServerState->PlayerInfo.CostumeColor = CostumeColor;
	}
}

// 이현정 : 서버에 준비 완료 알림
void APlayPlayerController::CallReady()
{
	Server_NotifyReadyForTravel();
}

// 이현정 : 서버에 준비 완료 알림
void APlayPlayerController::Server_NotifyReadyForTravel_Implementation()
{
	APlayPlayerState* MyState = GetPlayerState<APlayPlayerState>();
	if (MyState)
	{
		MyState->SetbReadyToTravelTrue();

		const FString ControllerName = GetName();
		const bool bReady = MyState->GetbReadyToTravel();

		UE_LOG(FALL_DEV_LOG, Log,
			TEXT("PlayPlayerController :: Server_NotifyReadyForTravel :: 컨트롤러: %s, 준비 상태: %s"),
			*ControllerName,
			bReady ? TEXT("True") : TEXT("False"));
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("Server_NotifyReadyForTravel :: PlayerState가 nullptr입니다."));
	}
}

void APlayPlayerController::Client_CallReadyAfterTravel_Implementation()
{
	// 다시 딜레이 호출
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &APlayPlayerController::CallReady, 1.0f, false);
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

// 이현정 : 클라이언트의 정보 세팅 및 동기화
void APlayPlayerController::Server_SetClientPlayerInfo_Implementation(const FString& _NickName,	const FString& _Top, const FString& _Bot, const FString& _Color)
{
	APlayPlayerState* ClientPlayerState = GetPlayerState<APlayPlayerState>();
	if (!ClientPlayerState) return;

	FPlayerInfo NewInfo = ClientPlayerState->PlayerInfo;
	NewInfo.NickName = _NickName;
	NewInfo.CostumeTOP = _Top;
	NewInfo.CostumeBOT = _Bot;
	NewInfo.CostumeColor = _Color;

	// 서버에 직접 세팅
	ClientPlayerState->PlayerInfo = NewInfo;

	// 클라이언트에게도 동기화
	ClientPlayerState->MCAST_ApplyPlayerInfo(NewInfo);
}

// 서버 → 클라이언트 : 승자 정보 전달용
void APlayPlayerController::Client_ReceiveWinnerInfo_Implementation(const FWinnerInfo& _Info)
{
	UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	if (GI)
	{
		GI->InsSetWinnerInfo(_Info);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayPlayerController :: Client_ReceiveWinnerInfo :: 클라이언트에 승자 정보 저장 - %s"), *_Info.NickName);
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayPlayerController :: Client_ReceiveWinnerInfo :: GameInstance가 nullptr입니다."));
	}
}

// 이현정 : 클라이언트들 EndLevel로 이동
void APlayPlayerController::Client_TravelToEndLevel_Implementation()
{
	if (IsLocalController() && GetWorld())
	{
		ClientTravel(TEXT("/Game/BP/Level/02_End/EndLevel"), ETravelType::TRAVEL_Absolute);
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayPlayerController :: Client_TravelToEndLevel :: 클라이언트가 EndLevel로 이동합니다."));
	}
}

// 이현정 : 팀전 : 타이틀로 돌아가
void APlayPlayerController::Client_TravelToTitleLevel_Implementation()
{
	if (IsLocalController() && GetWorld())
	{
		ClientTravel("/Game/BP/Level/00_Title/TitleLevel", ETravelType::TRAVEL_Absolute);
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayPlayerController :: Client_TravelToTitleLevel :: 클라이언트가 TitleLevel로 이동합니다."));
	}
}

// 다음 레벨로 이동하게 해주세요 (서버에게 요청)
void APlayPlayerController::Server_RequestSetCanMoveLevel_Implementation(bool _b)
{
	APlayGameMode* PlayMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	if (PlayMode)
	{
		PlayMode->SetCanMoveLevel(_b);
	}
}

