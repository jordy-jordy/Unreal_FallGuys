// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "Unreal_FallGuys.h"
#include "Global/BaseGameInstance.h"
#include "Mode/01_Play/PlayGameMode.h"
#include "Mode/01_Play/PlayPlayerState.h"
#include "Mode/01_Play/PlayCharacter.h"


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

void APlayPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 관전 중일 때만 회전 동기화
	if (GetViewTarget() && GetViewTarget() != GetPawn())
	{
		APlayCharacter* TargetCharacter = Cast<APlayCharacter>(GetViewTarget());
		if (TargetCharacter != nullptr)
		{
			FRotator TargetCamRot = TargetCharacter->ReplicatedCameraRotation;

			// 카메라 회전 반영
			SetControlRotation(TargetCamRot);
		}
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

		//UE_LOG(FALL_DEV_LOG, Log,
		//	TEXT("PlayPlayerController :: Server_NotifyReadyForTravel :: 컨트롤러: %s, 준비 상태: %s"),
		//	*ControllerName,
		//	bReady ? TEXT("True") : TEXT("False"));
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
		if (InputAction_NextSpectate)
		{
			EnhancedInput->BindAction(InputAction_NextSpectate, ETriggerEvent::Started, this, &APlayPlayerController::OnNextSpectate);
		}
		if (InputAction_PrevSpectate)
		{
			EnhancedInput->BindAction(InputAction_PrevSpectate, ETriggerEvent::Started, this, &APlayPlayerController::OnPrevSpectate);
		}
	}
}

void APlayPlayerController::OnNextSpectate()
{
	if (!IsLocalController()) return;
	APlayCharacter* MyCharacter = Cast<APlayCharacter>(GetPawn());
	APlayPlayerState* MyState = MyCharacter->GetPlayerState<APlayPlayerState>();
	if (MyCharacter == nullptr || MyState == nullptr) return;

	if (MyState->PlayerInfo.bIsSpectar == true)
	{
		++MyCharacter->SpectateTargetIndex;
		MyCharacter->C2S_RequestSetViewByIndex(MyCharacter->SpectateTargetIndex);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("OnNextSpectate :: 인덱스 증가: %d"), MyCharacter->SpectateTargetIndex);
	}
}

void APlayPlayerController::OnPrevSpectate()
{
	if (!IsLocalController()) return;
	APlayCharacter* MyCharacter = Cast<APlayCharacter>(GetPawn());
	APlayPlayerState* MyState = MyCharacter->GetPlayerState<APlayPlayerState>();
	if (MyCharacter == nullptr || MyState == nullptr) return;

	if (MyState->PlayerInfo.bIsSpectar == true)
	{
		--MyCharacter->SpectateTargetIndex;
		MyCharacter->C2S_RequestSetViewByIndex(MyCharacter->SpectateTargetIndex);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("OnNextSpectate :: 인덱스 감소: %d"), MyCharacter->SpectateTargetIndex);
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

// 일반 스테이지 : 뷰 타겟을 바꿔줌
void APlayPlayerController::Client_SetViewTargetByTag_Implementation(FName _TargetTag)
{
	bool bFound = false;
	int32 ActorCount = 0;

	for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
	{
		APlayCharacter* PlayerCharacter = *It;
		++ActorCount;

		if (PlayerCharacter)
		{
			APlayPlayerState* PS = PlayerCharacter->GetPlayerState<APlayPlayerState>();
			if (PS && PS->PlayerInfo.Tag == _TargetTag)
			{
				SetViewTargetWithBlend(PlayerCharacter, 0.0f); // 블렌딩 없이 바로 전환

				// 세팅 완료
				SettedRandomTarget = true;
				// 서버에게 완료 알림
				Server_NotifySettedRandomTarget(SettedRandomTarget);

				UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayPlayerController :: Client_SetViewTargetByTag :: 성공 → 태그: %s, 타겟: %s"),
					*_TargetTag.ToString(), *PlayerCharacter->GetName());

				bFound = true;
				break;
			}
			else
			{
				FString StateTag = PS ? PS->PlayerInfo.Tag.ToString() : TEXT("NoState");
				UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayPlayerController :: Client_SetViewTargetByTag :: 검사중 → 태그: %s | 현재 액터: %s | 상태태그: %s"),
					*_TargetTag.ToString(),
					*PlayerCharacter->GetName(),
					*StateTag);
			}
		}
	}

	if (!bFound)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayPlayerController :: Client_SetViewTargetByTag :: 실패 → 태그: %s | 전체 액터 수: %d"),
			*_TargetTag.ToString(), ActorCount);
	}
}

// 세팅 완료 했다
void APlayPlayerController::Server_NotifySettedRandomTarget_Implementation(bool _Value)
{
	SettedRandomTarget_server = _Value;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayPlayerController :: 서버에서 뷰 타겟 세팅 완료 확인 :: %s"), *GetName());
}

// 결과 화면 : 뷰 타겟을 바꿔줌
void APlayPlayerController::ClientWhoHidden_SetViewTargetByTag_Implementation(FName _TargetTag)
{
	bool bFound = false;
	int32 ActorCount = 0;

	for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
	{
		APlayCharacter* PlayerCharacter = *It;
		++ActorCount;

		if (PlayerCharacter)
		{
			APlayPlayerState* PS = PlayerCharacter->GetPlayerState<APlayPlayerState>();
			if (PS && PS->PlayerInfo.Tag == _TargetTag)
			{
				SetViewTargetWithBlend(PlayerCharacter, 0.0f); // 블렌딩 없이 바로 전환

				// 세팅 완료
				SettedTarget = true;
				// 서버에게 완료 알림
				Server_NotifySettedTarget(SettedTarget);

				UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayPlayerController :: Client_SetViewTargetByTag :: 성공 → 태그: %s, 타겟: %s"),
					*_TargetTag.ToString(), *PlayerCharacter->GetName());


				bFound = true;
				break;
			}
			else
			{
				FString StateTag = PS ? PS->PlayerInfo.Tag.ToString() : TEXT("NoState");
				UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayPlayerController :: Client_SetViewTargetByTag :: 검사중 → 태그: %s | 현재 액터: %s | 상태태그: %s"),
					*_TargetTag.ToString(),
					*PlayerCharacter->GetName(),
					*StateTag);
			}
		}
	}

	if (!bFound)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayPlayerController :: Client_SetViewTargetByTag :: 실패 → 태그: %s | 전체 액터 수: %d"),
			*_TargetTag.ToString(), ActorCount);
	}
}

void APlayPlayerController::Server_NotifySettedTarget_Implementation(bool _Value)
{
	SettedTarget_server = _Value;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayPlayerController :: 서버에서 뷰 타겟 세팅 완료 확인 :: %s"), *GetName());
}


void APlayPlayerController::Client_SetFailPlayerResultView(FName _Tag)
{
	// 저장된 타겟 태그로 뷰 설정
	ClientWhoHidden_SetViewTargetByTag(_Tag);

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayPlayerController :: Client_SetFailPlayerResultView :: 결과 화면 :: 저장된 타겟으로 카메라 세팅 | 타겟 태그: %s"),
		*_Tag.ToString());
}

void APlayPlayerController::Client_SetFailPlayerStageView(FName _Tag)
{
	// 저장된 타겟 태그로 뷰 설정
	Client_SetViewTargetByTag(_Tag);

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayPlayerController :: Client_SetFailPlayerStageView :: 일반 화면 :: 저장된 타겟으로 카메라 세팅 | 타겟 태그: %s"),
		*_Tag.ToString());
}
