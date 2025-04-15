// Fill out your copyright notice in the Description page of Project Settings.


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

	// 클라이언트에서 자기 GameInstance 정보 → 서버 PlayerState에 전달
	if (IsLocalController())
	{
		UBaseGameInstance* GI = Cast<UBaseGameInstance>(GetGameInstance());
		if (GI)
		{
			Server_SetPlayerInfoFromClient(
				GI->InsGetNickname(),
				GI->InsGetCostumeTop(),
				GI->InsGetCostumeBot(),
				GI->InsGetCostumeColor()
			);
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

// 이현정 : 승리한 플레이어의 정보를 전달하기 위함
void APlayPlayerController::Server_SetPlayerInfoFromClient_Implementation(
	const FString& _NickName,
	const FString& _Top,
	const FString& _Bot,
	const FString& _Color)
{
	APlayPlayerState* PS = GetPlayerState<APlayPlayerState>();
	if (!PS) return;

	FPlayerInfo NewInfo = PS->PlayerInfo;
	NewInfo.NickName = _NickName;
	NewInfo.CostumeTOP = _Top;
	NewInfo.CostumeBOT = _Bot;
	NewInfo.CostumeColor = _Color;

	// 서버에 직접 세팅
	PS->PlayerInfo = NewInfo;

	// 클라이언트에게도 동기화
	PS->MCAST_ApplyPlayerInfo(NewInfo);
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
