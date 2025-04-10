// Fill out your copyright notice in the Description page of Project Settings.


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


APlayGameMode::APlayGameMode()
{
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

	if (true == bInvalidConnect)
	{
		_ErrorMessage = TEXT("접속 제한: 게임 인원이 가득찼습니다.");
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: PreLogin :: 게임 인원이 가득차 접속이 거절되었습니다."));
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PreLogin END ======= "));
}
#pragma endregion

#pragma region PlayGameMode :: PostLogin :: 플레이어 접속시 실행
void APlayGameMode::PostLogin(APlayerController* _NewPlayer)
{
	Super::PostLogin(_NewPlayer);

	// 서버장이 아닐시 리턴
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin START ======= "));

	APlayGameState* FallState = nullptr;
	APlayPlayerState* PlayerState = nullptr;
	UBaseGameInstance* GameInstance = nullptr;

	// 게임 스테이트, 플레이어 스테이트, 게임 인스턴스 세팅
	if (!CheckEssentialObjects(_NewPlayer, FallState, PlayerState, GameInstance)) { return; }

	// 인원 카운팅
	FallState->AddConnectedPlayers();
	int ConnectingPlayer = FallState->GetConnectedPlayers();
	UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: PostLogin :: 새로운 플레이어가 접속 했습니다. 현재 접속 인원 : %d"), ConnectingPlayer);

	// 결과 레벨인지 확인
	bMODEIsResultLevel = GameInstance->bIsResultLevel;
	FallState->SetGameStateIsResultLevel(bMODEIsResultLevel);

	// 이전 스테이지에서 넘어온 경우 복원, 아니면 초기화
	if (GameInstance->IsMovedLevel)
	{
		RestorePlayerInfo(_NewPlayer, PlayerState, GameInstance);
	}
	else
	{
		InitPlayerInfo(_NewPlayer, PlayerState, FallState, GameInstance);
	}

	// 접속 여부 bool값 true로 변경
	GameInstance->InsSetbIsConnectedTrue();

	// 인원 수 체크
	CheckNumberOfPlayer(FallState);
	
	// 시네마틱과 접속 제한 세팅 호출
	StartCinematicIfReady(FallState);

	// 게임 인스턴스에서 스테이지의 종료 조건을 가져옴
	MODE_CurStageResultStatus = GameInstance->InsGetStageEndCondition();

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin END ======= "));
}
#pragma endregion

#pragma region PlayGameMode :: PostLogin 에서 실행되는 함수들
// 필수 데이터 세팅
bool APlayGameMode::CheckEssentialObjects(APlayerController* _NewPlayer, APlayGameState*& _OutFallState, APlayPlayerState*& _OutPlayerState, UBaseGameInstance*& _OutGameInstance)
{
	_OutFallState = GetGameState<APlayGameState>();
	if (!_OutFallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: CheckEssentialObjects :: GameState가 nullptr 입니다."));
		return false;
	}

	_OutPlayerState = Cast<APlayPlayerState>(_NewPlayer->PlayerState);
	if (!_OutPlayerState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: CheckEssentialObjects :: PlayerState가 nullptr 입니다."));
		return false;
	}

	_OutGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (!_OutGameInstance)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: CheckEssentialObjects :: GameInstance가 nullptr 입니다."));
		return false;
	}

	return true;
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
		TEXT("%s - UniqueID = %s, Tag(Info) = %s, Tag(Controller) = %s"),
		*_Prefix,
		*_Info.UniqueID,
		*TagFromInfo,
		*TagFromController);
}

// 세로운 플레이어 정보 세팅
void APlayGameMode::InitPlayerInfo(APlayerController* _NewPlayer, APlayPlayerState* _PlayerState, APlayGameState* _FallState, UBaseGameInstance* _GameInstance)
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 첫 스테이지 - 신규 플레이어 정보 세팅"));

	// 플레이어 태그 생성
	FName UniqueTag = GenerateUniquePlayerTag(_NewPlayer, _FallState->PlayerInfoArray.Num());

	// 플레이어 닉네임 가져옴
	FString PlayerNickname = _GameInstance->InsGetNickname();

	// 플레이어 정보 세팅
	_PlayerState->SetPlayerInfo(UniqueTag, PlayerNickname);

	LogPlayerInfo(TEXT("PlayGameMode :: PostLogin :: 신규 플레이어 정보 세팅"), _PlayerState->PlayerInfo, _NewPlayer);
}

// 기존 플레이어 정보 복구
void APlayGameMode::RestorePlayerInfo(APlayerController* _NewPlayer, APlayPlayerState* _PlayerState, UBaseGameInstance* _GameInstance)
{
	const FString PlayerUniqueID = _PlayerState->GetUniqueId()->ToString();

	FPlayerInfo RestoredInfo;
	_GameInstance->InsGetBackedUpPlayerInfo(PlayerUniqueID, RestoredInfo);

	if (bMODEIsResultLevel)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode:: PostLogin :: 결과 화면 - 기존 플레이어 정보 복구"));
	}
	else
	{
		if (RestoredInfo.Status == EPlayerStatus::SUCCESS)
		{
			RestoredInfo.Status = EPlayerStatus::DEFAULT;
		}
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode:: PostLogin :: 게임 스테이지 - 플레이어 정보 리셋"));
	}

	// 인포 복구
	_PlayerState->PlayerInfo = RestoredInfo;
	_NewPlayer->Tags.Add(RestoredInfo.Tag);

	LogPlayerInfo(TEXT("PlayGameMode :: PostLogin :: 플레이어 정보 복구 완료"), RestoredInfo, _NewPlayer);
}

// 인원 충족 했는지 체크
void APlayGameMode::CheckNumberOfPlayer(APlayGameState* _PlayState)
{
	if (_PlayState->GetConnectedPlayers() >= UFallConst::MinPlayerCount)
	{
		bNumberOfPlayer = true;
	}
	else
	{
		bNumberOfPlayer = false;
	}
}

// 시네마틱과 접속 제한 세팅 호출
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

// 레벨 시네마틱 시작을 호출하는 함수
void APlayGameMode::CallLevelCinematicStart(APlayGameState* _PlayState)
{
	_PlayState->SetCanStartLevelCinematic();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 레벨 시네마틱이 실행됩니다."));
}
#pragma endregion

#pragma region PlayGameMode :: BeginPlay :: 게임이 시작되는 곳
void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) { return; } // 서버에서만 실행

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode BeginPlay START ======= "));

	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	APlayGameState* FallState = GetGameState<APlayGameState>();

	// 게임 인스턴스에서 현재 스테이지의 에셋 이름을 가져옴
	MODE_CurLevelAssetName = GameInstance->InsGetCurLevelAssetName();
	// 게임 스테이트에 스테이지 에셋 이름을 전달
	FallState->SetPlayLevelAssetName(MODE_CurLevelAssetName);

	// 게임 인스턴스에서 현재 스테이지의 이름을 가져옴
	MODE_CurLevelName = GameInstance->InsGetCurLevelName();
	// 게임 스테이트에 스테이지 이름을 전달
	FallState->SetPlayLevelName(MODE_CurLevelName);

	// 게임 인스턴스에서 현재 스테이지 타입을 가져옴
	MODE_CurStageType = GameInstance->InsGetCurStageType();
	// 게임 스테이트에 스테이지 타입을 전달
	FallState->SetCurStageType(MODE_CurStageType);

	// 게임 인스턴스에서 현재 스테이지 페이즈를 가져옴
	MODE_CurStagePhase = GameInstance->InsGetCurStagePhase();
	// 게임 스테이트에 스테이지 페이즈를 전달
	FallState->SetCurStagePhase(MODE_CurStagePhase);

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
		1.0f,
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
	if (bNumberOfPlayer == false) { return; }

	APlayGameState* FallState = GetGameState<APlayGameState>();

	// 현 레벨이 결과 화면인 경우 : 시네마틱, 카운트 다운 바로 종료 처리 및 종료 트리거 모두 true 세팅
	if (bMODEIsResultLevel == true)
	{
		FallState->SetIsLevelCinematicEnd(true);
		bCountDownEnd = true;

		SetEndCondition_Trigger();
		bCanMoveLevel = true;
	}
	else
	{
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
		if (FallState->GetIsLevelCinematicEnd() == false) { return; }

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

	// 인원도 찼고, 레벨 시네마틱도 끝났고, 카운트 다운도 끝났으니까 게임 시작 가능
	if (bNumberOfPlayer == true && FallState->GetIsLevelCinematicEnd() == true && bCountDownEnd == true)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: BeginPlay :: 게임 시작 조건 충족. StartGame 호출"));

		// 게임 시작
		StartGame();

		// 조건 초기화 (중복 실행 방지)
		bNumberOfPlayer = false;
		bCountDownEnd = false;

		// 타이머 제거
		GetWorldTimerManager().ClearTimer(GameStartConditionTimer);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: BeginPlay :: CheckStartConditions 함수 종료"));
	}
}

// 상태가 디폴트인 플레이어 수
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
		}
	}
	return Count;
}

// 목표 골인 인원 수 제어
void APlayGameMode::ControllFinishPlayer()
{
	if (MODE_CurStageResultStatus == EPlayerStatus::SUCCESS)
	{
		FinishPlayer_Race();
	}
	else if (MODE_CurStageResultStatus == EPlayerStatus::FAIL)
	{
		FinishPlayer_Survive();
	}
}

// 목표 골인 인원 수 : 레이싱
void APlayGameMode::FinishPlayer_Race()
{
	switch (MODE_CurStagePhase)
	{
	case EStagePhase::STAGE_1:
		if		(DefaultPlayerCount <= 2) { SetFinishPlayerCount(1); }
		else if (DefaultPlayerCount == 3) { SetFinishPlayerCount(2); }
		else if (DefaultPlayerCount <= 5) { SetFinishPlayerCount(3); }
		else	{ SetFinishPlayerCount(DefaultPlayerCount / 2); }
		break;

	case EStagePhase::STAGE_2:
		if		(DefaultPlayerCount <= 2) { SetFinishPlayerCount(1); }
		else if (DefaultPlayerCount <= 5) { SetFinishPlayerCount(2); }
		else	{ SetFinishPlayerCount((DefaultPlayerCount / 2) / 2); }
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
	switch (MODE_CurStagePhase)
	{
	case EStagePhase::STAGE_1:
		if		(DefaultPlayerCount <= 1) { SetFinishPlayerCount(0); }
		else if (DefaultPlayerCount <= 3) { SetFinishPlayerCount(1); }
		else if (DefaultPlayerCount <= 5) { SetFinishPlayerCount(2); }
		else	{ SetFinishPlayerCount(DefaultPlayerCount / 2); }
		break;

	case EStagePhase::STAGE_2:
		if		(DefaultPlayerCount <= 1) { SetFinishPlayerCount(0); }
		else if (DefaultPlayerCount <= 2) { SetFinishPlayerCount(1); }
		else if (DefaultPlayerCount <= 5) { SetFinishPlayerCount(3); }
		else	{ SetFinishPlayerCount((DefaultPlayerCount / 2) / 2); }
		break;

	case EStagePhase::STAGE_3:
		if		(DefaultPlayerCount <= 1) { SetFinishPlayerCount(0); }
		else	{ SetFinishPlayerCount(1); }
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
	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, [this]()
		{
			for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
			{
				APlayCharacter* PlayerCharacter = *It;
				if (PlayerCharacter)
				{
					PlayerCharacter->S2M_SetCanMoveTrue();
				}
			}
		}, 0.2f, false); // 0.2초 뒤에 한 번 실행

	bPlayerMoving = true;
}

// 이현정 : 25.04.02 : 동기화 함수로 수정 : 골인 인원 +1 카운팅
void APlayGameMode::OnPlayerFinished()
{
	if (!HasAuthority()) return;

	CurFinishPlayer += 1;

	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	FallState->SetGameStateCurFinishPlayer(CurFinishPlayer);

	if (CurFinishPlayer >= FinishPlayer && IsEndGame == false)
	{
		SetEndCondition_Trigger();
	}
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
	if (!HasAuthority()) { return; }

	// 개인전이 아니면 여기서 끝
	if (MODE_CurStageType != EStageType::SOLO) { return; }

	// 서버 트래블 활성화 됐으면 여기서 끝
	if (StartedServerTravel) return;

	// 모든 조건이 true 가 되었을 때 서버 트래블 활성화
	if (IsEndGame && bPlayerStatusChanged && bPlayerInfosBackUp && bNextLevelDataSetted && bCanMoveLevel)
	{
		StartedServerTravel = true;

		// 결과 화면이 아닌 경우 결과 화면으로 이동
		if (!bMODEIsResultLevel)
		{
			ServerTravelToRaceOver();
		}
		else
		{
			// 결과 화면일 경우 30초 타이머 후 호출
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: Tick :: 결과 화면이므로 30초 후 다음 레벨로 이동합니다."));
			GetWorldTimerManager().SetTimer(ResultTravelTimerHandle, this, &APlayGameMode::ServerTravelToNextRandLevel, 30.0f, false);
		}
	}
}
#pragma endregion

#pragma region PlayGameMode :: Tick 에서 실행되는 함수들
// 게임 종료 트리거
void APlayGameMode::SetEndCondition_Trigger()
{
	if (IsEndGame == true) { return; }
	IsEndGame = true;

	// 공통 종료 로직
	SetEndCondition_Common();

	// 개인전 종료 로직
	SetEndCondition_Solo();
}

// 개인전 및 팀전 공통 종료 로직
void APlayGameMode::SetEndCondition_Common()
{
	// 결과 화면이 아닐때만
	if (!bMODEIsResultLevel)
	{
		// 레벨 종료 UI 띄워
		if (!bShowedLevelEndUI)
		{
			APlayGameState* FallState = GetGameState<APlayGameState>();
			FallState->MCAST_WidgetDelegate(TEXT("GameOver"));
			bShowedLevelEndUI = true;
		}

		// 캐릭터 멈추게
		if (bPlayerMoving)
		{
			SetCharacterMoveImPossible();
			bPlayerMoving = false;
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
	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, [this]()
		{
			for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
			{
				APlayCharacter* PlayerCharacter = *It;
				if (PlayerCharacter)
				{
					PlayerCharacter->S2M_SetCanMoveFalse();
				}
			}
		}, 0.2f, false); // 0.2초 뒤에 한 번 실행
}

// 개인전 종료 로직
void APlayGameMode::SetEndCondition_Solo()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	
	// 결과 화면이 아닐때만
	if (!bMODEIsResultLevel)
	{
		// 플레이어 상태를 변경한 이력이 없을때만
		if (!bPlayerStatusChanged)
		{
			// 남은 플레이어 상태 일괄 변경
			ChangeDefaultPlayersTo();
		}

		// 실패자에게 DropOrder 배정
		FallState->SetDropOrder();

		// 플레이어 인포를 백업한 이력이 없을때만
		if (!bPlayerInfosBackUp)
		{
			// 플레이어 정보 백업
			BackUpPlayersInfo();
		}
	}

	bPlayerStatusChanged = true;
	bPlayerInfosBackUp = true;

	// 개인전 세팅을 한 이력이 없을때만
	if (!bNextLevelDataSetted)
	{
		// 다음 레벨에 대한 정보 세팅
		SetNextSoloLevelData();
		bNextLevelDataSetted = true;
	}
}

// 플레이어 정보 백업
void APlayGameMode::BackUpPlayersInfo()
{
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	APlayGameState* PlayGameState = GetGameState<APlayGameState>();

	if (!GameInstance && !PlayGameState) { return; }

	// 백업하기 전에 비워주자
	GameInstance->PlayerInfoBackup.Empty();

	// 현재 게임 상태 가져오기
	for (FPlayerInfoEntry& PlayerEntry : PlayGameState->PlayerInfoArray)
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
	switch (MODE_CurStagePhase)
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

// 남은 플레이어의 상태 일괄 변경
void APlayGameMode::ChangeDefaultPlayersTo()
{
	if (MODE_CurStageResultStatus == EPlayerStatus::SUCCESS)
	{
		SetDefaultPlayersToFail();
	}
	else if (MODE_CurStageResultStatus == EPlayerStatus::FAIL)
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
			PState->SetPlayerStatus(EPlayerStatus::FAIL);
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
			PState->SetPlayerStatus(EPlayerStatus::SUCCESS);
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: SetDefaultPlayersToSuccess :: SUCCESS 처리됨 - %s"), *PState->PlayerInfo.Tag.ToString());
		}
	}
	// 상태 바꾼 것을 동기화
	SyncPlayerInfo();
}
#pragma endregion

// 개인전용 : 중간 결과창으로 이동
void APlayGameMode::ServerTravelToRaceOver()
{
	if (!HasAuthority()) { return; }

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 서버트래블 감지 :: %s 화면으로 이동합니다."), *NextLevel);

	GetWorld()->ServerTravel(NextLevel, false);
}

// 개인전용 : 다음 스테이지로 이동
void APlayGameMode::ServerTravelToNextRandLevel()
{
	if (!HasAuthority()) { return; }

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 서버트래블 감지 :: 다음 스테이지로 이동합니다."));

	GetWorld()->ServerTravel(UFallGlobal::GetRandomLevelWithOutPawn(), false);
}
