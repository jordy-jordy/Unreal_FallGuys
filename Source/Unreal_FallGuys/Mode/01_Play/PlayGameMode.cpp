﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameMode.h"
#include "Kismet/GameplayStatics.h" 
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

#include <Unreal_FallGuys.h>
#include <Global/FallConst.h>
#include <Global/BaseGameInstance.h>
#include <Mode/01_Play/PlayGameState.h>
#include <Mode/01_Play/PlayPlayerState.h>
#include <Mode/01_Play/PlayCharacter.h>
#include <Mode/01_Play/PlayPlayerController.h>


APlayGameMode::APlayGameMode()
{
	bUseSeamlessTravel = true;
	MaxPlayerCount = UFallConst::MinPlayerCount;
}

#pragma region PlayGameMode :: PreLogin :: 플레이어 접속시 가장 먼저 실행
void APlayGameMode::PreLogin(
	const FString& _Options,
	const FString& _Address,
	const FUniqueNetIdRepl& _UniqueId,
	FString& _ErrorMessage
)
{
	Super::PreLogin(_Options, _Address, _UniqueId, _ErrorMessage);

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PreLogin START ======= "));

	// 대기 인원 +1
	++WaitingPlayerCount;

	if ((WaitingPlayerCount + ConnectedPlayers) > MaxPlayerCount || true == bInvalidConnect)
	{
		_ErrorMessage = TEXT("접속 제한: 게임 인원이 가득찼습니다.");
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: PreLogin :: 게임 인원이 가득차 접속이 거절되었습니다."));
	}

	// 대기 인원 -1
	--WaitingPlayerCount;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PreLogin END ======= "));
}
#pragma endregion

#pragma region PlayGameMode :: Common :: 공통 처리 로직
// 필수 데이터 세팅
bool APlayGameMode::SetupCommonEssentialData(APlayerController* _NewPlayer, APlayGameState*& _OutFallState, APlayPlayerState*& _OutPlayerState, UBaseGameInstance*& _OutGameInstance)
{
	_OutFallState = GetGameState<APlayGameState>();
	if (!_OutFallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("SetupPlayerObjects :: GameState가 nullptr입니다."));
		return false;
	}

	_OutPlayerState = Cast<APlayPlayerState>(_NewPlayer->PlayerState);
	if (!_OutPlayerState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("SetupPlayerObjects :: PlayerState가 nullptr입니다."));
		return false;
	}

	_OutGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (!_OutGameInstance)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("SetupPlayerObjects :: GameInstance가 nullptr입니다."));
		return false;
	}

	// 레벨 정보 세팅 (공통)
	CurLevelInfo_Mode = _OutGameInstance->InsGetCurLevelInfo();
	_OutFallState->CurLevelInfo_GameState = CurLevelInfo_Mode;

	return true;
}

// 플레이어 인원 플러스
void APlayGameMode::AddPlayerCount(APlayGameState* _FallState)
{
	// 최대 접속자 수를 게임 스테이트에 세팅
	_FallState->SetStateMaxPlayerCount(MaxPlayerCount);

	// 플레이어 수 증가
	++ConnectedPlayers;
	_FallState->SetConnectedPlayers(ConnectedPlayers);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: PostLogin :: 새로운 플레이어가 접속 했습니다. 현재 접속 인원 : %d"), ConnectedPlayers);
}

// 인원 충족 했는지 체크
void APlayGameMode::CheckPlayersCount()
{
	if (ConnectedPlayers >= MaxPlayerCount)
	{
		bNumberOfPlayer = true;

		// 인원수 찼으니 접속제한 할건지 세팅
		SetPlayerAccessInvalid();
	}
	else
	{
		bNumberOfPlayer = false;
	}
}

// 접속 제한 세팅
void APlayGameMode::SetPlayerAccessInvalid()
{
	if (true == UFallConst::UsePlayerLimit)
	{
		bInvalidConnect = true;
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 접속 제한 활성화됨 - 추가 접속 불가"));
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 접속 제한 비활성화 - 추가 접속 허용"));
	}
}

// 플레이어 등록이 완료 된 후 실행되는 공통로직
void APlayGameMode::PostInitializePlayer(APlayGameState* _FallState)
{
	// 시네마틱 세팅
	StartCinematicIfReady(_FallState);
}

// 시네마틱 호출
void APlayGameMode::StartCinematicIfReady(APlayGameState* _FallState)
{
	// 인원수가 아직 안 찼으면 리턴
	if (bNumberOfPlayer == false) { return; }

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 게임 플레이를 위한 인원이 충족되었습니다."));

	if (!bMODEIsResultLevel)
	{
		FTimerDelegate LevelCinematicReadyTimer;
		LevelCinematicReadyTimer.BindUFunction(this, FName("CallLevelCinematicStart"), _FallState);

		GetWorldTimerManager().SetTimer(
			SetLevelCinematicStartTimer,
			LevelCinematicReadyTimer,
			UFallConst::LevelCinematicReady,
			false
		);

		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: %.0f초 뒤 레벨 시네마틱 실행 예정"), UFallConst::LevelCinematicReady);
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 결과 화면이므로 레벨 시네마틱 실행 안함"));
	}
}

// 레벨 시네마틱 시작을 호출하는 함수
void APlayGameMode::CallLevelCinematicStart(APlayGameState* _PlayState)
{
	_PlayState->SetCanStartLevelCinematic();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 레벨 시네마틱이 실행됩니다."));
}
#pragma endregion

#pragma region PlayGameMode :: PostLogin :: 플레이어 최초 접속시 실행
void APlayGameMode::PostLogin(APlayerController* _NewPlayer)
{
	Super::PostLogin(_NewPlayer);

	// 서버장이 아닐시 리턴
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin START ======= "));

	APlayGameState* FallState = nullptr;
	APlayPlayerState* PlayerState = nullptr;
	UBaseGameInstance* GameInstance = nullptr;

	// 공통 데이터 처리 - 게임 스테이트, 플레이어 스테이트, 게임 인스턴스, 레벨 데이터 세팅
	if (!SetupCommonEssentialData(_NewPlayer, FallState, PlayerState, GameInstance)) { return; }

	// 인원 카운팅
	AddPlayerCount(FallState);

	// 인원 수 체크 + 접속 제한 설정
	CheckPlayersCount();

	// 최초 접속 로직
	HandleFirstTimeLogin(_NewPlayer, PlayerState, FallState, GameInstance);

	// 시네마틱 시작 호출
	PostInitializePlayer(FallState);

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin END ======= "));
}
#pragma endregion

#pragma region PlayGameMode :: PostLogin 에서 실행되는 함수들
// 최초 접속 로직 : 플레이어 등록
void APlayGameMode::HandleFirstTimeLogin(APlayerController* _NewPlayer, APlayPlayerState* _PlayerState, APlayGameState* _FallState, UBaseGameInstance* _GameInstance)
{
	// 플레이어 등록
	InitPlayerInfo(_NewPlayer, _PlayerState, _FallState, _GameInstance);

	// 접속 여부 true
	_GameInstance->InsSetbIsConnectedTrue();
}

// 세로운 플레이어 정보 세팅
void APlayGameMode::InitPlayerInfo(APlayerController* _NewPlayer, APlayPlayerState* _PlayerState, APlayGameState* _FallState, UBaseGameInstance* _GameInstance)
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 첫 스테이지 - 신규 플레이어 정보 세팅"));

	// 플레이어 태그 생성
	FName UniqueTag = GenerateUniquePlayerTag(_NewPlayer, _FallState->GetPlayerInfoArray().Num());

	// 플레이어 정보 세팅
	_PlayerState->SetPlayerTag(UniqueTag);

	// 동기화 한번 해줌
	SyncPlayerInfo();

	LogPlayerInfo(TEXT("PlayGameMode :: PostLogin :: 신규 플레이어 정보 세팅"), _PlayerState->PlayerInfo, _NewPlayer);
}

// 플레이어 태그 생성
FName APlayGameMode::GenerateUniquePlayerTag(APlayerController* _NewPlayer, int32 _PlayerIndex)
{
	FName UniqueTag = FName(*FString::Printf(TEXT("Player%d"), _PlayerIndex));

	if (!_NewPlayer->Tags.Contains(UniqueTag))
	{
		_NewPlayer->Tags.Add(UniqueTag);
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("GenerateUniquePlayerTag :: 중복된 태그 감지: %s"), *UniqueTag.ToString());
	}

	return UniqueTag;
}

// 플레이어 인포 로그
void APlayGameMode::LogPlayerInfo(const FString& _Prefix, const FPlayerInfo& _Info, APlayerController* _Controller)
{
	const FString TagFromInfo = _Info.Tag.IsNone() ? TEXT("NoTag") : _Info.Tag.ToString();

	FString TagFromController = TEXT("NoTag");
	if (_Controller && _Controller->Tags.Num() > 0)
	{
		TagFromController = _Controller->Tags[0].ToString();
	}

	UE_LOG(FALL_DEV_LOG, Log,
		TEXT("%s - UniqueID = %s, Tag(Info) = %s, Tag(Controller) = %s, DropOrder = %d"),
		*_Prefix,
		*_Info.UniqueID,
		*TagFromInfo,
		*TagFromController,
		_Info.DropOrder);
}
#pragma endregion

#pragma region PlayGameMode :: HandleSeamlessTravelPlayer :: 레벨 이동 후 실행
// 첫번째 스테이지 이후 - 레벨 이동시
void APlayGameMode::HandleSeamlessTravelPlayer(AController*& _NewController)
{
	Super::HandleSeamlessTravelPlayer(_NewController);

	// 서버장이 아닐시 리턴
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode HandleSeamlessTravelPlayer START ======= "));

	APlayerController* NewPlayerController = Cast<APlayerController>(_NewController);
	if (!NewPlayerController) return;

	APlayGameState* FallState = nullptr;
	APlayPlayerState* PlayerState = nullptr;
	UBaseGameInstance* GameInstance = nullptr;

	// 공통 데이터 처리 - 게임 스테이트, 플레이어 스테이트, 게임 인스턴스, 레벨 데이터 세팅
	if (!SetupCommonEssentialData(NewPlayerController, FallState, PlayerState, GameInstance)) { return; }

	// 인원 카운팅
	AddPlayerCount(FallState);

	// 인원 수 체크 + 접속 제한 설정
	CheckPlayersCount();

	// 결과 레벨인지 세팅
	GetIsResultLevel(PlayerState, FallState, GameInstance);

	// 이전 스테이지에서 넘어온 경우 복원, 아니면 초기화
	if (GetIsLevelMoved(GameInstance))
	{
		RestorePlayerInfo(NewPlayerController, PlayerState, FallState, GameInstance);
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("뭔가 잘못됐습니다."));
	}

	// 시네마틱 시작 호출
	PostInitializePlayer(FallState);

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode HandleSeamlessTravelPlayer END ======= "));
}
#pragma endregion

#pragma region PlayGameMode :: HandleSeamlessTravelPlayer 에서 실행되는 함수들
// 결과 화면인지 게임 인스로부터 가져옴
void APlayGameMode::GetIsResultLevel(APlayPlayerState* _PlayerState, APlayGameState* _FallState, UBaseGameInstance* _GameInstance)
{
	bMODEIsResultLevel = _GameInstance->bIsResultLevel;
	_FallState->SetGameStateIsResultLevel(bMODEIsResultLevel);
	_PlayerState->SetIsResultLevel(bMODEIsResultLevel);
}

// 레벨 이동 했는지 게임 인스로부터 가져옴
bool APlayGameMode::GetIsLevelMoved(UBaseGameInstance* _GameInstance)
{
	bMODEIsLevelMoved = _GameInstance->IsMovedLevel;
	return bMODEIsLevelMoved;
}

// 기존 플레이어 정보 복구
void APlayGameMode::RestorePlayerInfo(APlayerController* _NewPlayer, APlayPlayerState* _PlayerState, APlayGameState* _FallState, UBaseGameInstance* _GameInstance)
{
	const FString PlayerUniqueID = _PlayerState->GetUniqueId()->ToString();

	FPlayerInfo RestoredInfo;
	_GameInstance->InsGetBackedUpPlayerInfo(PlayerUniqueID, RestoredInfo);

	if (bMODEIsResultLevel) // 결과 화면
	{
		// 성공한 사람들의 관전자 상태를 리셋
		if (RestoredInfo.Status == EPlayerStatus::SUCCESS)
		{
			RestoredInfo.bIsSpectar = false; // 관전자 초기화
		}
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode:: PostLogin :: 결과 화면 - 기존 플레이어 정보 복구"));
	}
	else // 일반 스테이지
	{
		if (CurLevelInfo_Mode.LevelType == EStageType::SOLO)
		{
			// 성공한 사람들의 관전자 상태를 리셋 및 상태 리셋
			if (RestoredInfo.Status == EPlayerStatus::SUCCESS)
			{
				RestoredInfo.Status = EPlayerStatus::DEFAULT;
				RestoredInfo.bIsSpectar = false;
			}
			// 실패한 사람들의 관전자 상태 복구 및 결과 레벨에서 숨김 처리
			else if (RestoredInfo.Status == EPlayerStatus::FAIL)
			{
				RestoredInfo.bCanHiddenAtResult = true;
				RestoredInfo.bIsSpectar = true; // 관전자 세팅
			}
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode:: PostLogin :: 개인전 입니다 - 성공한 플레이어 정보 리셋"));
		}
		else if (CurLevelInfo_Mode.LevelType == EStageType::TEAM)
		{
			RestoredInfo.Status = EPlayerStatus::DEFAULT;
			RestoredInfo.Team = ETeamType::NONE;
			RestoredInfo.bIsSpectar = false; // 관전자 초기화
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode:: PostLogin :: 팀전 입니다 - 플레이어 팀 정보 리셋"));
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode:: PostLogin :: 게임 모드가 잘못됐습니다."));
		}
	}

	// 인포 복구
	_PlayerState->PlayerInfo = RestoredInfo;
	_PlayerState->S2M_SetPlayInfo(_PlayerState->PlayerInfo);
	_NewPlayer->Tags.Add(RestoredInfo.Tag);

	// 유저 리스트 업데이트 해줌
	_FallState->UpdateAlivePlayers();

	// 이전 스테이지에서 실패한 유저 리스트 복구
	_FallState->RestoreFailPlayersInfo();

	// 동기화 한번 해줌
	SyncPlayerInfo();

	LogPlayerInfo(TEXT("PlayGameMode :: PostLogin :: 플레이어 정보 복구 완료"), RestoredInfo, _NewPlayer);
}
#pragma endregion

#pragma region PlayGameMode :: BeginPlay :: 게임이 시작되는 곳
void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	if (GEngine && GEngine->IsEditor())
	{
		GEngine->Exec(GetWorld(), TEXT("net.AllowPIESeamlessTravel 1"));
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: APlayGameMode :: PIE 환경 → 심리스 트래블 설정 적용"));
	}
#endif

	if (!HasAuthority()) return; // 서버에서만 실행

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode BeginPlay START ======= "));
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: GameMode 주소: %p"), this);

	// 게임 시작을 위한 조건을 주기적으로 체크
	GetWorldTimerManager().SetTimer(
		GameStartConditionTimer,
		this,
		&APlayGameMode::CheckStartConditions,
		1.0f,  // 1초마다 검사
		true   // 반복 실행
	);

	// 플레이어 정보 지속 동기화용 타이머
	GetWorldTimerManager().SetTimer(
		SyncPlayerInfoTimer,
		this,
		&APlayGameMode::SyncPlayerInfo,
		5.0f,
		true
	);

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode BeginPlay END ======= "));
}
#pragma endregion

#pragma region PlayGameMode :: BeginPlay 에서 실행되는 함수들
// 게임 시작을 위한 조건 체크
void APlayGameMode::CheckStartConditions()
{
	// 인원이 안찼으면 리턴
	if (bNumberOfPlayer == false) return;
	
	// 모든 플레이어의 컨트롤러와 캐릭터가 준비되지 않았다면 리턴
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!bAllPlayerReadyToGame)
	{
		if (!bSettedPlayersRepliTimer && AreAllClientsReadyToGame())
		{
			// 타이머 중복 방지: 이미 예약된 타이머가 있으면 무시
			if (!GetWorldTimerManager().IsTimerActive(AllPlayerReadyTimerHandle))
			{
				FTimerDelegate ReadyDelegate;
				ReadyDelegate.BindLambda([this]()
					{
						APlayGameState* FallState = GetGameState<APlayGameState>();
						if (FallState)
						{
							bAllPlayerReadyToGame = true;
							FallState->SetbAllPlayerReadyToGame_State(true);
							UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: BeginPlay :: 모든 컨트롤러 및 캐릭터가 준비 완료됨."));
						}
					});

				// 딜레이를 건 뒤 실행
				GetWorldTimerManager().SetTimer(AllPlayerReadyTimerHandle, ReadyDelegate, 2.0f, false);
				bSettedPlayersRepliTimer = true;
			}
		}
		return;
	}

	// 현 레벨이 결과 화면인 경우 : 시네마틱, 카운트 다운 바로 종료 처리 및 종료 트리거 모두 true 세팅
	if (bMODEIsResultLevel == true)
	{
		if (CurLevelInfo_Mode.LevelType == EStageType::SOLO)
		{
			// 실패자의 관전자 활성화
			// SetSpectar_RESULT();
		}

		FallState->SetIsLevelCinematicEnd(true);
		bCountDownEnd = true;
		bCanMoveLevel = true;
	}
	else
	{
		if (CurLevelInfo_Mode.LevelType == EStageType::SOLO)
		{
			// 실패자의 관전자 활성화
			SetSpectar_STAGE();
		}

		// 인원수 조정
		if (bSettedGoalCount == false)
		{
			// 디폴트 상태의 플레이어 수 카운트
			DefaultPlayerCount = GetDefaultPlayerCount();
			// 디폴트 상태의 플레이어 수에 따른 목표 횟수 설정
			ControllFinishPlayer();
			bSettedGoalCount = true;
			FallState->SetGameStateGoalCountSetted(bSettedGoalCount);
		}

		// 시네마틱이 안끝났으면 리턴
		if (bCinematicEND == false) return;

		// 카운트 다운 사용할거야?
		if (UFallConst::UseCountDown == true)
		{
			if (bCountDownStarted == false)
			{
				// 카운트 다운 핸들 활성화
				StartCountdownTimer();
				FallState->MCAST_WidgetDelegate(TEXT("StartCount"));
				bCountDownStarted = true;
			}
			// 카운트 다운이 안끝났으면 리턴
			if (bCountDownEnd == false) return;
		}
		else
		{
			// 카운트다운을 사용하지 않으면 바로 끝난 것으로 처리
			bCountDownEnd = true;
		}
	}

	// 인원도 찼고, 플레이어들도 준비 됐고, 레벨 시네마틱도 끝났고, 카운트 다운도 끝났으니까 게임 시작 가능
	if (bNumberOfPlayer == true && bAllPlayerReadyToGame == true && bCinematicEND == true && bCountDownEnd == true)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: BeginPlay :: 게임 시작 조건 충족. StartGame 호출"));

		// 게임 시작
		StartGame();

		// 조건 초기화 (중복 실행 방지)
		bNumberOfPlayer = false;
		bCinematicEND = false;
		bCountDownEnd = false;

		// 게임 시작 조건을 체크하는 타이머 제거
		GetWorldTimerManager().ClearTimer(GameStartConditionTimer);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: BeginPlay :: CheckStartConditions 함수 종료"));
	}
}

// 모든 플레이어가 게임할 준비됐는지 체크
bool APlayGameMode::AreAllClientsReadyToGame()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return false;

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (!PState || !PState->GetControllerReadyToGame())
		{
			return false;
		}
	}

	return true;
}

// 상태가 디폴트인 플레이어 수 카운트 및 수집
int32 APlayGameMode::GetDefaultPlayerCount()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: GetDefaultPlayerCount :: GameState가 nullptr 입니다."));
		return 0;
	}

	int32 Count = 0;

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->GetPlayerStateStatus() == EPlayerStatus::DEFAULT)
		{
			++Count;

			FPlayerInfoEntry Entry(PState->PlayerInfo.UniqueID, PState->PlayerInfo);

			UE_LOG(FALL_DEV_LOG, Log,
				TEXT("PlayGameMode :: GetDefaultPlayerCount :: UID = %s, Tag = %s → 디폴트 상태로 등록"),
				*Entry.UniqueID,
				*Entry.PlayerInfo.Tag.ToString());
		}
	}
	return Count;
}

// 목표 골인 인원 수 제어
void APlayGameMode::ControllFinishPlayer()
{
	if (CurLevelInfo_Mode.EndCondition == EPlayerStatus::SUCCESS)
	{
		FinishPlayer_Race();
	}
	else if (CurLevelInfo_Mode.EndCondition == EPlayerStatus::FAIL)
	{
		FinishPlayer_Survive();
	}
}

// 목표 골인 인원 수 : 레이싱
void APlayGameMode::FinishPlayer_Race()
{
	switch (CurLevelInfo_Mode.CurStagePhase)
	{
	case EStagePhase::STAGE_1:
		if (DefaultPlayerCount <= 2) { SetFinishPlayerCount(1); }
		else if (DefaultPlayerCount <= 4) { SetFinishPlayerCount(DefaultPlayerCount - 1); }
		else if (DefaultPlayerCount <= 5) { SetFinishPlayerCount(3); }
		else { SetFinishPlayerCount(DefaultPlayerCount / 2); }
		break;

	case EStagePhase::STAGE_2:
		if (DefaultPlayerCount <= 2) { SetFinishPlayerCount(1); }
		else if (DefaultPlayerCount <= 5) { SetFinishPlayerCount(DefaultPlayerCount - 1); }
		else { SetFinishPlayerCount((DefaultPlayerCount / 2) / 2); }
		break;

	case EStagePhase::STAGE_3:
		SetFinishPlayerCount(1);
		break;

	case EStagePhase::FINISHED:
	default:
		break;
	}
}

// 목표 골인 인원 수 : 생존
void APlayGameMode::FinishPlayer_Survive()
{
	switch (CurLevelInfo_Mode.CurStagePhase)
	{
	case EStagePhase::STAGE_1:
		if (DefaultPlayerCount <= 1) { SetFinishPlayerCount(0); }
		else if (DefaultPlayerCount <= 4) { SetFinishPlayerCount(1); }
		else if (DefaultPlayerCount <= 5) { SetFinishPlayerCount(2); }
		else { SetFinishPlayerCount(DefaultPlayerCount / 2); }
		break;

	case EStagePhase::STAGE_2:
		if (DefaultPlayerCount <= 1) { SetFinishPlayerCount(0); }
		else if (DefaultPlayerCount <= 4) { SetFinishPlayerCount(1); }
		else if (DefaultPlayerCount <= 5) { SetFinishPlayerCount(2); }
		else { SetFinishPlayerCount((DefaultPlayerCount / 2) / 2); }
		break;

	case EStagePhase::STAGE_3:
		if (DefaultPlayerCount <= 1) { SetFinishPlayerCount(0); }
		else { SetFinishPlayerCount(DefaultPlayerCount - 1); }
		break;

	case EStagePhase::FINISHED:
	default:
		break;
	}
}

// 게임 시작전 :: 카운트다운 핸들 활성화
void APlayGameMode::StartCountdownTimer()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: %.0f 초 대기후 카운트 다운이 시작됩니다."), UFallConst::FallCountDownStandByTime);

	// 설정한 대기 시간이 끝난 뒤 카운트다운 시작
	FTimerHandle DelayTimer;
	GetWorldTimerManager().SetTimer(DelayTimer, this, &APlayGameMode::StartCountdown, UFallConst::FallCountDownStandByTime, false);
}

// 게임 시작전 :: 카운트다운 시작 (대기 후 실행)
void APlayGameMode::StartCountdown()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	float Time = FallState->GetCountDownTime();
	if (!FallState) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 카운트다운 진행 시작. 초기 값: %.0f"), Time);

	if (Time <= 0)
	{
		FallState->SetCountDownTime(10.0f);
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 카운트다운 값이 0이거나 음수라 기본값(10초)으로 설정"));
	}

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &APlayGameMode::UpdateCountdown, 1.0f, true);
}

// 게임 시작전 :: 카운트다운 진행 (매초 실행)
void APlayGameMode::UpdateCountdown()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	// 시간 감소 → 새로운 시간 저장
	FallState->MinusCountDownTime(1.0f);
	const float NewTime = FallState->GetCountDownTime();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 카운트다운 : %.0f"), NewTime);

	// float 비교는 안전하게 처리 (±0.01 허용)
	if (NewTime <= KINDA_SMALL_NUMBER)
	{
		// 타이머 정지
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		// 카운트 다운 완료 처리
		bCountDownEnd = true;
		FallState->SetIsCountDownOverTrue();

		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 카운트다운 종료"));
	}
}

// 게임 시작
void APlayGameMode::StartGame()
{
	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();

	// 게임 시작됐음
	bGameStarted = true;
	FallState->SetGameStateGameStarted(bGameStarted);
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: BeginPlay :: 게임이 시작되었습니다."));

	// 캐릭터 움직이게 처리
	SetCharacterMovePossible();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: BeginPlay :: 캐릭터 이동이 가능합니다."));
}

// 캐릭터 이동 가능하게 세팅
void APlayGameMode::SetCharacterMovePossible()
{
	for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
	{
		APlayCharacter* PlayerCharacter = *It;
		if (PlayerCharacter && PlayerCharacter->GetPlayerState<APlayPlayerState>()->PlayerInfo.Status == EPlayerStatus::DEFAULT)
		{
			PlayerCharacter->S2M_SetCanMoveTrue();
		}
	}

	// 캐릭터 움직임 가능하다
	bPlayerMoving = true;
}

// 이현정 : 25.04.02 : 동기화 함수로 수정 : 골인 목표 인원 수 세팅
void APlayGameMode::SetFinishPlayerCount(int _p)
{
	if (!HasAuthority()) return;

	FinishPlayer = _p;

	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	FallState->SetGameStateFinishPlayer(_p);
}

// 플레이어 인포 동기화
void APlayGameMode::SyncPlayerInfo()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: PostLogin :: GameState가 nullptr 입니다."));
		return;
	}
	FallState->SyncPlayerInfoFromPlayerState();
}
#pragma endregion

#pragma region PlayGameMode :: Tick :: 게임이 진행되는 곳
void APlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 서버만 실행
	if (!HasAuthority()) return;

	// 모든 플레이어의 컨트롤러와 캐릭터가 준비되지 않았다면 리턴하도록 해
	if (!bAllPlayerReadyToGame) return;

	// 게임이 시작되지 않았다면 리턴하도록 해
	if (!bGameStarted) return;

	// 개인전이 아니면 여기서 끝
	if (CurLevelInfo_Mode.LevelType != EStageType::SOLO) return;

	// 서버 트래블 활성화 됐으면 여기서 끝
	if (StartedServerTravel) return;

	// 결과 화면이 아니고, 성공 조건이 0명이면 바로 성공 처리 : 부전승
	if (!bMODEIsResultLevel && FinishPlayer == 0 && !bSetWinbyDefault)
	{
		APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
		SetEndCondition_Trigger(FallState);
		bSetWinbyDefault = true;
	}

	// 결과 화면이면 바로 성공 처리
	if (bMODEIsResultLevel && !bCalledEndTriggerAtResult)
	{
		APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
		SetEndCondition_Trigger(FallState);
		bCalledEndTriggerAtResult = true;
	}

	// 모든 조건이 true 가 되었을 때 서버 트래블 활성화
	if (IsEndGame && bPlayerStatusChanged && bPlayerInfosBackUp && bNextLevelDataSetted && bCanMoveLevel)
	{
		// 결과 화면이 아닌 경우 결과 화면으로 이동
		if (!bMODEIsResultLevel)
		{
			// 서버 트래블 활성화
			StartedServerTravel = true;
			ServerTravelToRaceOver();
		}
		else if (bMODEIsResultLevel && !bPassedResultLevel && CurLevelInfo_Mode.CurStagePhase != EStagePhase::STAGE_3_RESULT)
		{
			// 결과 화면에서 넘어가도 된다는 콜이 오기 전까진 리턴
			if (!bCanMoveResultLevel) return;

			// 서버 트래블 활성화
			StartedServerTravel = true;
			// 결과 화면에서 넘어갔단다.
			bPassedResultLevel = true;

			// 결과 화면일 경우 5초 타이머 후 호출
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: Tick :: 레벨로부터 레벨 이동 콜을 받았습니다. 5초 후 다음 레벨로 이동합니다."));
			GetWorldTimerManager().SetTimer(ResultTravelTimerHandle, this, &APlayGameMode::ServerTravelToNextRandLevel, 5.0f, false);
		}
		else
		{
			// 결과 화면에서 넘어가도 된다는 콜이 오기 전까진 리턴
			if (!bCanMoveResultLevel) return;

			// 서버 트래블 활성화
			StartedServerTravel = true;

			// 최종 승리자 체크
			MarkWinnersBeforeEndLevel();

			// 엔드레벨로 이동
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: Tick :: 레벨로부터 레벨 이동 콜을 받았습니다. 5초 후 최종 결과 화면으로 이동합니다."));
			GetWorldTimerManager().SetTimer(ResultTravelTimerHandle, this, &APlayGameMode::ClientTravelToEndLevel, 5.0f, false);
		}
	}
}
#pragma endregion

#pragma region PlayGameMode :: Tick 에서 실행되는 함수들
// 이현정 : 25.04.02 : 동기화 함수로 수정 - 게임종료 트리거
void APlayGameMode::OnPlayerFinished(APlayCharacter* _Character)
{
	// 서버장이 아닐시 리턴
	if (!HasAuthority()) return;

	// 게임 시작 안했으면 리턴
	if (!bGameStarted) return;
	// 게임 끝났으면 리턴
	if (IsEndGame) return;

	// 일단 게임 스테이트를 가져오고
	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	if (!FallState) return;

	// 결승선에 닿은 캐릭터의 스테이트를 가져옴
	APlayPlayerState* PlayerState = _Character->GetPlayerState<APlayPlayerState>();
	if (!PlayerState) return;
	// 이미 실패한 유저는 리턴
	if (PlayerState->PlayerInfo.Status == EPlayerStatus::FAIL) return;

	// 이동을 막음
	_Character->S2M_SetCanMoveFalse();

	// 관전자 중에서 이 캐릭터를 보고 있는 사람들 → 타겟 변경
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayPlayerController* SpectatorPC = Cast<APlayPlayerController>(It->Get());
		APlayCharacter* SpectatorPC_Character = Cast<APlayCharacter>(SpectatorPC->GetPawn());
		APlayPlayerState* SpectatorPC_State = Cast<APlayPlayerState>(SpectatorPC_Character->GetPlayerState());

		if (SpectatorPC_State && SpectatorPC_State->PlayerInfo.CurSpectateTargetTag == PlayerState->PlayerInfo.Tag)
		{
			// 타겟 바꿔줌
			SetRandomViewForClient(SpectatorPC);
		}
	}

	if (CurLevelInfo_Mode.EndCondition == EPlayerStatus::SUCCESS)
	{
		// 레이싱 : 성공 처리
		PlayerState->SetPlayerStatus(EPlayerStatus::SUCCESS);
	}
	else if (CurLevelInfo_Mode.EndCondition == EPlayerStatus::FAIL)
	{
		// 생존 : 실패 처리
		PlayerState->SetPlayerStatus(EPlayerStatus::FAIL);
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: OnPlayerFinished :: 뭔가 잘못됨."));
		return;
	}

	// 관전자 모드를 켜줌
	PlayerState->SetPlayertoSpectar(true);
	APlayerController* FallController = Cast<APlayerController>(_Character->GetController());
	SetRandomViewForClient(FallController);

	// 메쉬 및 콜리전 투명화
	_Character->C2S_ApplySpectatorVisibilityAtGoalColl();

	// 결승선 or 킬존 닿은 플레이어 카운트 +1
	++CurFinishPlayer;
	FallState->SetGameStateCurFinishPlayer(CurFinishPlayer);

	if (CurFinishPlayer >= FinishPlayer && IsEndGame == false)
	{
		SetEndCondition_Trigger(FallState);
	}
}

// 게임 종료 트리거
void APlayGameMode::SetEndCondition_Trigger(APlayGameState* _FallState)
{
	// 게임 끝났으면 리턴
	if (IsEndGame == true) return;

	// 게임 종료 설정
	IsEndGame = true;
	_FallState->SetStateIsEndGameTrue();

	// 공통 종료 로직
	SetEndCondition_Common(_FallState);

	// 개인전 종료 로직
	SetEndCondition_Solo(_FallState);
}

// 개인전 및 팀전 공통 종료 로직
void APlayGameMode::SetEndCondition_Common(APlayGameState* _FallState)
{
	// 결과 화면이 아닐때만
	if (!bMODEIsResultLevel)
	{
		// 캐릭터 멈추게
		if (bPlayerMoving)
		{
			SetCharacterMoveImPossible();
			bPlayerMoving = false;
		}

		// 레벨 종료 UI 띄워
		if (!bShowedLevelEndUI)
		{
			_FallState->MCAST_WidgetDelegate(TEXT("GameOver"));
			bShowedLevelEndUI = true;
		}
	}

	// 동기화 타이머 해제
	if (!bSyncCleared)
	{
		GetWorldTimerManager().ClearTimer(SyncPlayerInfoTimer);
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: SetEndCondition_Common :: SyncPlayerInfo 타이머 제거"));
		bSyncCleared = true;
	}
}

// 캐릭터 이동 불가능하게 세팅
void APlayGameMode::SetCharacterMoveImPossible()
{
	for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
	{
		APlayCharacter* PlayerCharacter = *It;
		if (PlayerCharacter)
		{
			PlayerCharacter->S2M_SetCanMoveFalse();
		}
	}
}

// 개인전 종료 로직
void APlayGameMode::SetEndCondition_Solo(APlayGameState* _FallState)
{
	// 결과 화면이 아닐때만
	if (!bMODEIsResultLevel)
	{
		// 플레이어 상태를 변경한 이력이 없을때만
		if (!bPlayerStatusChanged)
		{
			// 남은 플레이어 상태 일괄 변경
			ChangeDefaultPlayersTo();
		}

		// 실패자에게 DropOrder 배정 및 실패한 유저 정보 백업
		_FallState->SetDropOrder();

		if (!bSettedRandomViewTarget)
		{
			// 성공한 플레이어를 배열에 저장
			UpdateSuccessPlayerInfoArray();
			PrepareSpectatorTargets();
		}
	}

	bPlayerStatusChanged = true;
	bSettedRandomViewTarget = true;

	ResetAllControllersTargetStatus();
	
	// 플레이어 인포를 백업한 이력이 없을때만
	if (!bMODEIsResultLevel && !bPlayerInfosBackUp)
	{
		// 플레이어 정보 백업
		BackUpPlayersInfo();
	}

	bPlayerInfosBackUp = true;

	// 개인전 세팅을 한 이력이 없을때만
	if (!bNextLevelDataSetted)
	{
		// 다음 레벨에 대한 정보 세팅
		SetNextSoloLevelData();
		bNextLevelDataSetted = true;
	}
}

// 남은 플레이어의 상태 일괄 변경
void APlayGameMode::ChangeDefaultPlayersTo()
{
	if (CurLevelInfo_Mode.EndCondition == EPlayerStatus::SUCCESS)
	{
		SetDefaultPlayersToFail();
	}
	else if (CurLevelInfo_Mode.EndCondition == EPlayerStatus::FAIL)
	{
		SetDefaultPlayersToSuccess();
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: ChangeDefaultPlayersTo :: 게임 종료 조건이 뭔가 잘못 됨"));
	}
}

// Default 상태인 플레이어를 FAIL 상태로 변경
void APlayGameMode::SetDefaultPlayersToFail()
{
	// 마지막 동기화
	SyncPlayerInfo();

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: SetDefaultPlayersToFail :: GameState가 nullptr입니다."));
		return;
	}

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->GetPlayerStateStatus() == EPlayerStatus::DEFAULT)
		{
			PState->SetPlayerStatusOnEnd(EPlayerStatus::FAIL);
			PState->SetPlayertoSpectar(true);
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: SetDefaultPlayersToFail :: FAIL 처리됨 - %s"), *PState->PlayerInfo.Tag.ToString());
		}
	}
	// 상태 바꾼 것을 동기화
	SyncPlayerInfo();
}

// Default 상태인 플레이어를 SUCCESS 상태로 변경
void APlayGameMode::SetDefaultPlayersToSuccess()
{
	// 마지막 동기화
	SyncPlayerInfo();

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: SetDefaultPlayersToSuccess :: GameState가 nullptr입니다."));
		return;
	}

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->GetPlayerStateStatus() == EPlayerStatus::DEFAULT)
		{
			PState->SetPlayerStatusOnEnd(EPlayerStatus::SUCCESS);
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: SetDefaultPlayersToSuccess :: SUCCESS 처리됨 - %s"), *PState->PlayerInfo.Tag.ToString());
		}
	}
	// 상태 바꾼 것을 동기화
	SyncPlayerInfo();
}

// 성공한 플레이어들을 SuccessPlayerInfoArray에 저장
void APlayGameMode::UpdateSuccessPlayerInfoArray()
{
	// 마지막 동기화
	SyncPlayerInfo();

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: UpdateSuccessPlayerInfoArray :: GameState가 nullptr 입니다."));
		return;
	}

	FallState->SuccessPlayerInfoArray.Empty();

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->PlayerInfo.Status == EPlayerStatus::SUCCESS)
		{
			FPlayerInfoEntry Entry(PState->PlayerInfo.UniqueID, PState->PlayerInfo);
			FallState->SuccessPlayerInfoArray.Add(Entry);

			UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: UpdateSuccessPlayerInfoArray :: 성공 플레이어 등록 - %s"), *Entry.PlayerInfo.Tag.ToString());
		}
	}
}

// 실패한 플레이어들에게 관전 타겟 지정
void APlayGameMode::PrepareSpectatorTargets()
{
	// 마지막 동기화
	SyncPlayerInfo();

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	const TArray<FPlayerInfoEntry>& SuccessPlayers = FallState->SuccessPlayerInfoArray;

	if (SuccessPlayers.Num() == 0)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PrepareSpectatorTargets :: 성공한 플레이어 없음"));
		return;
	}

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->PlayerInfo.bCanHiddenAtResult == true || PState->PlayerInfo.Status == EPlayerStatus::FAIL)
		{
			int32 RandomIndex = FMath::RandRange(0, SuccessPlayers.Num() - 1);
			const FPlayerInfoEntry& TargetEntry = SuccessPlayers[RandomIndex];

			// 관전자 타겟 저장
			PState->PlayerInfo.SpectateTargetTag = TargetEntry.PlayerInfo.Tag;

			UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: PrepareSpectatorTargets :: 관전자 %s → 타겟 %s"),
				*PState->PlayerInfo.Tag.ToString(),
				*TargetEntry.PlayerInfo.Tag.ToString());
		}
	}

	// 마지막 동기화
	SyncPlayerInfo();
}

// 플레이어 정보 백업
void APlayGameMode::BackUpPlayersInfo()
{
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	APlayGameState* PlayGameState = GetGameState<APlayGameState>();

	if (!GameInstance && !PlayGameState) return;

	// 백업하기 전에 비워주자
	GameInstance->PlayerInfoBackup.Empty();

	// 현재 게임 상태 가져오기
	for (FPlayerInfoEntry& PlayerEntry : PlayGameState->GetPlayerInfoArray())
	{
		GameInstance->InsBackupPlayerInfo(PlayerEntry.UniqueID, PlayerEntry.PlayerInfo);
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: BackUpPlayersInfo :: 플레이어 정보 백업 완료 - UniqueID = %s, Tag = %s"),
			*PlayerEntry.UniqueID, *PlayerEntry.PlayerInfo.Tag.ToString());
	}
}

// 개인전 세팅
void APlayGameMode::SetNextSoloLevelData()
{
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());

	// 개인전인 경우
	switch (CurLevelInfo_Mode.CurStagePhase)
	{
	case EStagePhase::STAGE_1:
		GameInstance->InsSetCurStagePhase(EStagePhase::STAGE_1_RESULT);
		GameInstance->bIsResultLevel = true;
		break;

	case EStagePhase::STAGE_1_RESULT:
		GameInstance->InsSetCurStagePhase(EStagePhase::STAGE_2);
		GameInstance->bIsResultLevel = false;
		break;

	case EStagePhase::STAGE_2:
		GameInstance->InsSetCurStagePhase(EStagePhase::STAGE_2_RESULT);
		GameInstance->bIsResultLevel = true;
		break;

	case EStagePhase::STAGE_2_RESULT:
		GameInstance->InsSetCurStagePhase(EStagePhase::STAGE_3);
		GameInstance->bIsResultLevel = false;
		break;

	case EStagePhase::STAGE_3:
		GameInstance->InsSetCurStagePhase(EStagePhase::STAGE_3_RESULT);
		GameInstance->bIsResultLevel = true;
		break;

	case EStagePhase::STAGE_3_RESULT:
		GameInstance->InsSetCurStagePhase(EStagePhase::FINISHED);
		GameInstance->bIsResultLevel = true;
		break;

	default:
		break;
	}

	// 스테이지 전환 했음을 알림
	GameInstance->IsMovedLevel = true;
}

// 최종 승리 플레이어를 마킹
void APlayGameMode::MarkWinnersBeforeEndLevel()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->GetPlayerStateStatus() == EPlayerStatus::SUCCESS)
		{
			PState->SetIsWinner(true);

			UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: MarkWinnersBeforeEndLevel :: 승자 설정 완료 - %s"), *PState->PlayerInfo.Tag.ToString());

			// 승자 정보를 준비
			FWinnerInfo Info;
			Info.NickName = PState->PlayerInfo.NickName;
			Info.CostumeColor = PState->PlayerInfo.CostumeColor;
			Info.CostumeTop = PState->PlayerInfo.CostumeTOP;
			Info.CostumeBot = PState->PlayerInfo.CostumeBOT;

			// 모든 컨트롤러에게 승자 정보 브로드캐스트
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				APlayPlayerController* OtherPC = Cast<APlayPlayerController>(It->Get());
				if (OtherPC)
				{
					OtherPC->Client_ReceiveWinnerInfo(Info);
				}
			}
		}
	}
}
#pragma endregion

#pragma region PlayGameMode :: 서버 트래블 관련 함수들
// 개인전용 : 중간 결과창으로 이동
void APlayGameMode::ServerTravelToRaceOver()
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 서버트래블 감지 :: %s 화면으로 이동합니다."), *NextLevel);

	GetWorld()->ServerTravel(NextLevel, true);
}

// 개인전용 : 다음 스테이지로 이동
void APlayGameMode::ServerTravelToNextRandLevel()
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 서버트래블 감지 :: 다음 스테이지로 이동합니다."));

	GetWorld()->ServerTravel(UFallGlobal::GetRandomLevelWithOutPawn(), true);
}

// 개인전용 : 최종 결과창으로 이동
void APlayGameMode::ClientTravelToEndLevel()
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 서버에서 클라이언트들에게 EndLevel 트래블 명령 시작"));

	int32 ControllerIndex = 0;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It, ++ControllerIndex)
	{
		APlayPlayerController* PC = Cast<APlayPlayerController>(It->Get());
		if (PC == nullptr) continue;

		// 0번 컨트롤러는 서버장 → 트래블 제외
		if (ControllerIndex == 0)
		{
			UE_LOG(FALL_DEV_LOG, Log, TEXT("ServerTravelToEndLevel :: [0번] 서버장 트래블은 클라이언트들이 이동 된 뒤 처리됩니다."));
			continue;
		}

		// 클라이언트 먼저 트래블
		PC->Client_TravelToEndLevel();
	}

	// 서버장도 엔드 레벨로 가자
	ServerTravelToEndLevel();
}

// 개인전용 : 클라이언트 최종 결과창으로 이동 - 서버
void APlayGameMode::ServerTravelToEndLevel()
{
	APlayPlayerController* ServerHostPC = Cast<APlayPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (ServerHostPC == nullptr)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: ServerTravelToEndLevel :: 서버장 컨트롤러를 찾을 수 없습니다."));
		return;
	}

	// 0.2초 후 서버장 클라이언트 트래블
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 서버장이 0.2초 뒤 EndLevel로 ClientTravel 시작"));
	FTimerHandle ServerHostTravelHandle;
	GetWorldTimerManager().SetTimer(ServerHostTravelHandle, [ServerHostPC]()
		{
			ServerHostPC->ClientTravel(TEXT("/Game/BP/Level/02_End/EndLevel"), ETravelType::TRAVEL_Absolute);
		}, 0.2f, false);
}

#pragma endregion

// 일반 스테이지용
void APlayGameMode::SetSpectar_STAGE()
{
	for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
	{
		APlayCharacter* PlayerCharacter = *It;
		if (!PlayerCharacter) continue;

		APlayPlayerController* FallController = PlayerCharacter->GetController<APlayPlayerController>();
		APlayPlayerState* PS = PlayerCharacter->GetPlayerState<APlayPlayerState>();
		if (PS && PS->PlayerInfo.Status == EPlayerStatus::FAIL)
		{
			FString TagString = TEXT("");
			if (!FallController->SettedRandomTarget_server)
			{
				TagString = *PS->PlayerInfo.SpectateTargetTag.ToString();
				UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 일반 화면 :: 뷰타겟 변경 호출 | 대상자의 캐릭터: %s | 대상자의 닉네임: %s | 뷰타겟 태그: %s"),
					*PlayerCharacter->GetName(),
					*PS->PlayerInfo.NickName,
					*TagString
				);

				FallController->Client_SetFailPlayerStageView(PS->PlayerInfo.SpectateTargetTag);
			}
			PlayerCharacter->S2M_ApplySpectatorVisibilityAtPlay();
		}
	}
}

// 결과 화면용
void APlayGameMode::SetSpectar_RESULT()
{
	//for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
	//{
	//	APlayCharacter* PlayerCharacter = *It;
	//	if (!PlayerCharacter) continue;

	//	APlayPlayerController* FallController = PlayerCharacter->GetController<APlayPlayerController>();
	//	APlayPlayerState* PS = PlayerCharacter->GetPlayerState<APlayPlayerState>();
	//	if (PS && PS->PlayerInfo.bCanHiddenAtResult == true)
	//	{
	//		FString TagString = TEXT("");
	//		if (!FallController->SettedTarget_server)
	//		{
	//			FallController->Client_SetFailPlayerResultView(PS->PlayerInfo.SpectateTargetTag);
	//			TagString = *PS->PlayerInfo.SpectateTargetTag.ToString();

	//			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 결과 화면 :: 뷰타겟 변경 호출 | 컨트롤러: %s | 닉네임: %s | 뷰타겟 태그: %s"),
	//				*PlayerCharacter->GetName(),
	//				*PS->PlayerInfo.NickName,
	//				*TagString
	//			);
	//		}
	//		PlayerCharacter->S2M_ApplySpectatorVisibility();
	//		PlayerCharacter->SetActorLocation({ 0, -10000, 0 });
	//	}
	//}
}

void APlayGameMode::SetRandomViewForClient(APlayerController* _CurController)
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState || !_CurController) return;

	// 유저 리스트 업데이트 해줌
	FallState->UpdateAlivePlayers();
	TArray<APlayCharacter*> ValidTargets = FallState->GetAlivePlayers();

	if (ValidTargets.Num() == 0)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: SetRandomViewForClient :: 유효한 타겟 없음"));
		return;
	}

	// 랜덤 타겟 뽑기
	int32 RandomIndex = FMath::RandRange(0, ValidTargets.Num() - 1);
	APlayCharacter* RandomTarget = ValidTargets[RandomIndex];

	APlayPlayerController* PC = Cast<APlayPlayerController>(_CurController);
	if (RandomTarget && PC)
	{
		FName TargetTag = NAME_None;
		APlayPlayerState* PS = RandomTarget->GetPlayerState<APlayPlayerState>();
		if (PS)
		{
			TargetTag = PS->PlayerInfo.Tag;
		}

		// 현재 컨트롤러의 캐릭터
		APlayCharacter* FallPC = Cast<APlayCharacter>(PC->GetCharacter());
		APlayPlayerState* FallPCState = Cast<APlayPlayerState>(FallPC->GetPlayerState());
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 뷰 타겟 호출 - 랜덤 :: 서버 → 클라 :: 대상자의 태그: %s | 타겟 컨트롤러: %s | 타겟의 태그: %s"),
			*FallPCState->PlayerInfo.Tag.ToString(), *RandomTarget->GetName(), *TargetTag.ToString());
		
		PC->Client_SetViewTargetByTag(TargetTag);
	}
}

void APlayGameMode::SetViewForClientByIndex(APlayerController* _CurController, int32 _TargetIndex)
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState || !_CurController) return;

	// 유저 리스트 업데이트 해줌
	FallState->UpdateAlivePlayers();
	TArray<APlayCharacter*> ValidTargets = FallState->GetAlivePlayers();

	if (ValidTargets.Num() == 0)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: SetViewForClientByIndex :: 유효한 타겟 없음"));
		return;
	}

	// 인덱스 보정
	int32 ValidIndex = _TargetIndex % ValidTargets.Num();
	if (ValidIndex < 0) ValidIndex += ValidTargets.Num();
	// 다음 타겟
	APlayCharacter* TargetCharacter = ValidTargets[ValidIndex];

	APlayPlayerController* PC = Cast<APlayPlayerController>(_CurController);
	if (TargetCharacter && PC)
	{
		FName TargetTag = NAME_None;
		APlayPlayerState* PS = TargetCharacter->GetPlayerState<APlayPlayerState>();
		if (PS)
		{
			TargetTag = PS->PlayerInfo.Tag;
		}

		// 현재 컨트롤러의 캐릭터
		APlayCharacter* FallPC = Cast<APlayCharacter>(PC->GetCharacter());
		APlayPlayerState* FallPCState = Cast<APlayPlayerState>(FallPC->GetPlayerState());
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 뷰 타겟 호출 - 배열 순환 :: 서버 → 클라 :: 대상자의 태그: %s | 타겟 컨트롤러: %s | 타겟의 태그: %s | 인덱스: %d"),
			*FallPCState->PlayerInfo.Tag.ToString(), *TargetCharacter->GetName(), *TargetTag.ToString(), ValidIndex);

		PC->Client_SetViewTargetByTag(TargetTag);
	}
}

void APlayGameMode::ResetAllControllersTargetStatus()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayPlayerController* PC = Cast<APlayPlayerController>(It->Get());
		if (PC)
		{
			PC->SettedTarget = false;
			PC->Server_NotifySettedTarget(false);
			PC->SettedRandomTarget = false;
			PC->Server_NotifySettedRandomTarget(false);
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: ResetAllControllersTargetStatus :: 컨트롤러 %s 초기화 완료"), *PC->GetName());
		}
	}
}

APawn* APlayGameMode::SpawnDefaultPawnFor_Implementation(AController* _NewPlayer, AActor* _StartSpot)
{
	if (DefaultPawnClass == nullptr || _StartSpot == nullptr)
	{
		return Super::SpawnDefaultPawnFor_Implementation(_NewPlayer, _StartSpot);
	}

	const FVector SpawnLocation = _StartSpot->GetActorLocation();
	const FRotator SpawnRotation = _StartSpot->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(
		DefaultPawnClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (NewPawn == nullptr)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: SpawnDefaultPawnFor :: Pawn 스폰 실패"));
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: SpawnDefaultPawnFor :: %s 에 Pawn 스폰 완료 (회전: %s)"),
			*NewPawn->GetName(), *SpawnRotation.ToString());
	}

	return NewPawn;
}
