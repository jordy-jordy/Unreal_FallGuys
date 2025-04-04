﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameMode.h"
#include "Kismet/GameplayStatics.h" 
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/FallConst.h>
#include <Global/BaseGameInstance.h>
#include <Mode/01_Play/PlayGameState.h>
#include <Mode/01_Play/PlayPlayerState.h>
#include <Mode/01_Play/PlayCharacter.h>
#include <Level/01_Play/Actor/ImMovable/JumpShowDown/ShowDownStage.h>


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

	if (true == InvalidConnect)
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

	// GameState가 없을시 리턴
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) { UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: PostLogin :: GameState가 nullptr 입니다.")); return; }

	// PlayerState가 없을시 리턴
	APlayPlayerState* PlayerState = Cast<APlayPlayerState>(_NewPlayer->PlayerState);
	if (!PlayerState) { UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: PostLogin :: PlayerState가 nullptr 입니다.")); return; }

	// GameInstance가 없을시 리턴
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (!GameInstance) { UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: PostLogin :: GameInstance가 nullptr 입니다.")); return; }
	
	FString PlayerUniqueID = PlayerState->GetUniqueId()->ToString(); // UniqueID 얻음

	// 결과 화면인지 확인
	bMODEIsResultLevel = GameInstance->bIsResultLevel;

	// 첫 스테이지인지 확인
	if (GameInstance->IsMovedLevel)
	{
		FPlayerInfo RestoredInfo;
		GameInstance->InsGetBackedUpPlayerInfo(PlayerUniqueID, RestoredInfo);
		if (bMODEIsResultLevel) // 결과 화면
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 결과 화면입니다. 기존 플레이어 정보를 복구 합니다."));
			PlayerState->PlayerInfo = RestoredInfo;
		}
		else if (!bMODEIsResultLevel) // 게임 스테이지
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 게임 스테이지 입니다. 기존 플레이어 정보를 리셋 합니다."));
			RestoredInfo.Status = EPlayerStatus::DEFAULT;
			PlayerState->PlayerInfo = RestoredInfo;
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 있을 수 없는 조건. 로직 확인 필요"));
		}

		// 태그 복구
		_NewPlayer->Tags.Add(*RestoredInfo.Tag);
		FString TagString = (_NewPlayer->Tags.Num() > 0) ? _NewPlayer->Tags[0].ToString() : TEXT("태그 없음");

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: PostLogin :: 플레이어 정보 로드 완료 - UniqueID = %s, Tag = %s"),
			*RestoredInfo.UniqueID, *TagString);
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 첫 스테이지 입니다. 새 플레이어 정보를 세팅합니다."));

		// 태그 설정
		FString UniqueTag = FString::Printf(TEXT("Player%d"), FallState->PlayerInfoArray.Num());
		_NewPlayer->Tags.Add(*UniqueTag);
		// 안전하게 태그 값 가져오기
		FString TagString = (_NewPlayer->Tags.Num() > 0) ? _NewPlayer->Tags[0].ToString() : TEXT("태그 없음");

		// 닉네임 설정
		FString PlayerNickname = GameInstance->InsGetNickname();

		// 정보 세팅
		PlayerState->SetPlayerInfo(UniqueTag, PlayerNickname);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: PostLogin :: 신규 플레이어 정보 세팅 - UniqueID = %s, Tag = %s"),
			*PlayerState->PlayerInfo.UniqueID, *TagString);
	}

	// 접속 여부 bool값 true로 변경
	GameInstance->InsSetbIsConnectedTrue();

	// 현 스테이지가 무엇이지?
	MODECurrentStage = GameInstance->InsGetSavedStage();
	// 게임 시작 인원 수에 따른 목표 횟수 설정
	ControllFinishPlayer();
	// 게임 인스턴스에 저장된 레벨 이름을 게임 스테이트에 저장
	FallState->SavePlayLevelName(GameInstance->InsGetCurLevelName());
	// 게임 인스턴스에 저장된 레벨 에셋 이름을 게임 스테이트에 저장
	FallState->SavePlayLevelAssetName(GameInstance->InsGetCurLevelAssetName());

	// 인원 카운팅
	FallState->AddConnectedPlayers();
	int ConnectingPlayer = FallState->GetConnectedPlayers();
	UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: PostLogin :: 새로운 플레이어가 접속 했습니다. 현재 접속 인원 : %d"), ConnectingPlayer);

	// 인원 수 체크
	CheckNumberOfPlayer(FallState);
	if (true == pNumberOfPlayer)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 게임 플레이를 위한 인원이 충족되었습니다."));

		// 결과 화면이 아닌 경우에만 시네마틱 시작
		if (!bMODEIsResultLevel)
		{
			// 인원수가 찼을 시 설정한 시간 뒤에 시네마틱 시작
			FTimerDelegate LevelCinematicReadyTimer;
			LevelCinematicReadyTimer.BindUFunction(this, FName("CallLevelCinematicStart"), FallState);

			GetWorldTimerManager().SetTimer(
				SetLevelCinematicStartTimer,
				LevelCinematicReadyTimer,
				UFallConst::LevelCinematicReady,   // 설정된 시간 뒤에 실행
				false   // 반복 실행 false
			);
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: %.0f초 뒤에 레벨 시네마틱이 실행됩니다."), UFallConst::LevelCinematicReady);
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 결과 화면이므로 레벨 시네마틱이 실행되지 않습니다."));
		}

		if (true == UFallConst::UsePlayerLimit) // 인원이 찼고 인원 제한을 사용하는 경우 접속 제한 활성화
		{
			InvalidConnect = true;
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 접속 제한을 사용하므로 이후 접속이 제한됩니다."));
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 접속 제한을 사용하지 않습니다. 플레이어의 추가 접속이 가능합니다."));
		}
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin END ======= "));
}
#pragma endregion

#pragma region PlayGameMode :: PostLogin 에서 실행되는 함수들
// 인원 충족 했는지 체크
void APlayGameMode::CheckNumberOfPlayer(APlayGameState* _PlayState)
{
	if (_PlayState->GetConnectedPlayers() >= UFallConst::MinPlayerCount)
	{
		pNumberOfPlayer = true;
	}
	else
	{
		pNumberOfPlayer = false;
	}
}

// 레벨 시네마틱 시작을 호출하는 함수
void APlayGameMode::CallLevelCinematicStart(APlayGameState* _PlayState)
{
	_PlayState->SetCanStartLevelCinematic();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 레벨 시네마틱이 실행됩니다."));
}

// 목표 골인 인원 수 제어
void APlayGameMode::ControllFinishPlayer()
{
	int32 MinCount = UFallConst::MinPlayerCount;

	switch (MODECurrentStage)
	{
	case EStageType::STAGE_1:
		if (MinCount <= 2)
		{
			SetFinishPlayerCount(MinCount);
		}
		else if (MinCount <= 5)
		{
			SetFinishPlayerCount(3);
		}
		else
		{
			SetFinishPlayerCount(MinCount / 2);
		}
		break;

	case EStageType::STAGE_2:
		if (MinCount <= 2)
		{
			SetFinishPlayerCount(1);
		}
		else if (MinCount <= 5)
		{
			SetFinishPlayerCount(2);
		}
		else
		{
			SetFinishPlayerCount((MinCount / 2) / 2);
		}
		break;

	case EStageType::STAGE_3:
		SetFinishPlayerCount(1);
		break;

	case EStageType::FINISHED:
	default:
		break;
	}
}
#pragma endregion

#pragma region PlayGameMode :: BeginPlay :: 게임이 시작되는 곳
void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode BeginPlay START ======= "));

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
}
#pragma endregion

#pragma region PlayGameMode :: BeginPlay 에서 실행되는 함수들
// 게임 시작을 위한 조건 체크
void APlayGameMode::CheckStartConditions()
{
	// 인원이 안찼으면 리턴
	if (pNumberOfPlayer == false) { return; }

	// 현 레벨이 결과 화면인 경우 : 실패자 처리 하고 바로 게임 시작되게
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (bMODEIsResultLevel == true)
	{
		FallState->SetDropOrder();
		FallState->PrintFailPlayersInfo();

		FallState->SetIsLevelCinematicEnd(true);
		pCountDownEnd = true;
	}
	else // 결과 화면이 아닐때만
	{
		// 시네마틱이 안끝났으면 리턴
		if (FallState->GetIsLevelCinematicEnd() == false) { return; }

		// 카운트 다운 사용할거야?
		if (UFallConst::UseCountDown == true)
		{
			if (pCountDownStarted == false)
			{
				// 카운트 다운 핸들 활성화
				StartCountdownTimer();
				pCountDownStarted = true;
			}
			// 카운트 다운이 안끝났으면 리턴
			if (pCountDownEnd == false) return;
		}
		else
		{
			// 카운트다운을 사용하지 않으면 바로 끝난 것으로 처리
			pCountDownEnd = true;
		}
	}

	// 인원도 찼고, 레벨 시네마틱도 끝났고, 카운트 다운도 끝났으니까 게임 시작 가능
	if (pNumberOfPlayer == true && FallState->GetIsLevelCinematicEnd() == true && pCountDownEnd == true)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: BeginPlay :: 게임 시작 조건 충족. StartGame 호출"));

		// 게임 시작
		StartGame();

		// 조건 초기화 (중복 실행 방지)
		pNumberOfPlayer = false;
		pCountDownEnd = false;

		// 타이머 제거
		GetWorldTimerManager().ClearTimer(GameStartConditionTimer);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: BeginPlay :: CheckStartConditions 함수 종료"));
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
		pCountDownEnd = true;
		FallState->SetIsCountDownOverTrue();

		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: PostLogin :: 카운트다운 종료"));
	}
}

// 게임 시작
void APlayGameMode::StartGame()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: BeginPlay :: 게임이 시작되었습니다."));

	// 캐릭터 움직이게 처리
	SetCharacterMovePossible();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: BeginPlay :: 캐릭터 이동이 가능합니다."));

	// 스테이지 제한 시간 처리
	StartStageLimitTimer();
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

	pPlayerMoving = true;
}

// 스테이지 제한 시간 타이머 활성화
void APlayGameMode::StartStageLimitTimer()
{
	if (!HasAuthority()) return;

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	if (FallState->GetUseStageLimitTime() == false)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: BeginPlay :: 스테이지 제한 시간을 사용하지 않으므로 타이머 시작 안함"));
		return;
	}

	float Time = FallState->GetStageLimitTime();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: BeginPlay :: 스테이지 제한 시간 타이머 시작: %.2f초"), Time);

	GetWorldTimerManager().SetTimer(StageLimitTimerHandle, this, &APlayGameMode::OnStageLimitTimeOver, Time, false);
}

// 스테이지 제한 시간 오버 처리
void APlayGameMode::OnStageLimitTimeOver()
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: BeginPlay :: 제한 시간 초과! 스테이지를 종료합니다."));

	// 다음 맵 이동
	ServerTravelToNextMap(UFallGlobal::GetRandomLevelWithOutPawn());
}

// 이현정 : 25.04.02 : 동기화 함수로 수정 : 골인 인원 +1 카운팅
void APlayGameMode::OnPlayerFinished()
{
	if (!HasAuthority()) return;

	CurFinishPlayer += 1;

	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	FallState->SetGameStateCurFinishPlayer(CurFinishPlayer);
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

	// 이미 게임 종료면 처리 안함
	if (IsEndGame == true) { return; }

	// 게임 종료 조건 검사
	if (CurFinishPlayer >= FinishPlayer)
	{
		IsEndGame = true;

		// 동기화 타이머 해제
		GetWorldTimerManager().ClearTimer(SyncPlayerInfoTimer);
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: Tick :: 게임 종료 → SyncPlayerInfo 타이머 제거"));

		SetDefaultPlayersToFail();

		ServerTravelToNextMap(NextLevel);
	}
}
#pragma endregion

#pragma region PlayGameMode :: Tick 에서 실행되는 함수들
// Default 상태인 플레이어를 FAIL 상태로 변경
void APlayGameMode::SetDefaultPlayersToFail()
{
	// 마지막 동기화
	SyncPlayerInfo();

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: Tick :: GameState가 nullptr입니다."));
		return;
	}

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->GetPlayerStateStatus() == EPlayerStatus::DEFAULT)
		{
			PState->SetPlayerStatus(EPlayerStatus::FAIL);
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: Tick :: FAIL 처리됨 - %s"), *PState->PlayerInfo.Tag);
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
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: Tick :: GameState가 nullptr입니다."));
		return;
	}

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->GetPlayerStateStatus() == EPlayerStatus::DEFAULT)
		{
			PState->SetPlayerStatus(EPlayerStatus::SUCCESS);
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: Tick :: SUCCESS 처리됨 - %s"), *PState->PlayerInfo.Tag);
		}
	}
	// 상태 바꾼 것을 동기화
	SyncPlayerInfo();
}
#pragma endregion

void APlayGameMode::ServerTravelToNextMap(const FString& url)
{
	if (!HasAuthority()) { return; }

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 서버트래블 감지 ::"));

	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	APlayGameState* PlayGameState = GetGameState<APlayGameState>();
	if (GameInstance && PlayGameState)
	{
		// 백업하기 전에 비워주자
		GameInstance->PlayerInfoBackup.Empty();

		// 현재 게임 상태 가져오기
		for (FPlayerInfoEntry& PlayerEntry : PlayGameState->PlayerInfoArray)
		{
			GameInstance->InsBackupPlayerInfo(PlayerEntry.UniqueID, PlayerEntry.PlayerInfo);
			UE_LOG(FALL_DEV_LOG, Log, TEXT("ServerTravelToNextMap :: 플레이어 정보 백업 완료 - UniqueID = %s, Tag = %s"),
				*PlayerEntry.UniqueID, *PlayerEntry.PlayerInfo.Tag);
		}

		// 다음 스테이지 값을 미리 저장
		switch (MODECurrentStage)
		{
		case EStageType::STAGE_1:
			GameInstance->InsSetSavedStage(EStageType::STAGE_1_RESULT);
			GameInstance->bIsResultLevel = true;
			break;

		case EStageType::STAGE_1_RESULT:
			GameInstance->InsSetSavedStage(EStageType::STAGE_2);
			GameInstance->bIsResultLevel = false;
			break;

		case EStageType::STAGE_2:
			GameInstance->InsSetSavedStage(EStageType::STAGE_2_RESULT);
			GameInstance->bIsResultLevel = true;
			break;

		case EStageType::STAGE_2_RESULT:
			GameInstance->InsSetSavedStage(EStageType::STAGE_3);
			GameInstance->bIsResultLevel = false;
			break;

		case EStageType::STAGE_3:
			GameInstance->InsSetSavedStage(EStageType::FINISHED);
			GameInstance->bIsResultLevel = true; 
			break;

		default:
			break;
		}
		// 스테이지 전환 했음을 알림
		GameInstance->IsMovedLevel = true;
	}
	GetWorld()->ServerTravel(url, false);
}

