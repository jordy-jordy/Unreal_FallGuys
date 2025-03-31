// Fill out your copyright notice in the Description page of Project Settings.


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

void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode BeginPlay START ======= "));
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode BeginPlay END ======= "));
	}
}

void APlayGameMode::ServerTravelToNextMap(const FString& url)
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 서버트래블 감지 ::"));

	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	APlayGameState* PlayGameState = GetGameState<APlayGameState>();
	if (GameInstance && PlayGameState)
	{
		// 현재 게임 상태 가져오기
		for (const FPlayerInfoEntry& PlayerEntry : PlayGameState->PlayerInfoArray)
		{
			GameInstance->InsBackupPlayerInfo(PlayerEntry.UniqueID, PlayerEntry.PlayerInfo);
			UE_LOG(FALL_DEV_LOG, Log, TEXT("ServerTravelToNextMap :: 플레이어 정보 백업 완료 - UniqueID = %s, Tag = %s"),
				*PlayerEntry.UniqueID, *PlayerEntry.PlayerInfo.Tag);
		}

		// 다음 스테이지 값을 미리 저장
		switch (PlayGameState->CurrentStage)
		{
		case EStageType::STAGE_1:
			GameInstance->InsSetSavedStage(EStageType::STAGE_2);
			break;
		case EStageType::STAGE_2:
			GameInstance->InsSetSavedStage(EStageType::STAGE_3);
			break;
		case EStageType::STAGE_3:
			GameInstance->InsSetSavedStage(EStageType::FINISHED);
			break;
		default:
			break;
		}

		// 스테이지 전환 했음을 알림
		GameInstance->IsMovedLevel = true;
	}
	GetWorld()->ServerTravel(url, false);
}

void APlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 게임 종료 체크
	if (true == IsEndGame) return;

	// 골인한 플레이어 수와 목표 인원 수 체크
	if (CurFinishPlayer >= FinishPlayer)
	{
		IsEndGame = true;

		ServerTravelToNextMap(NextLevel);
	}
}

// 플레이어 인포 동기화
void APlayGameMode::SyncPlayerInfo_Implementation()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: GameState가 nullptr 입니다."));
		return;
	}
	FallState->SyncPlayerInfoFromPlayerState();
}

// 인원 충족 했는지 체크
void APlayGameMode::CheckNumberOfPlayer(APlayGameState* _PlayState)
{
	UBaseGameInstance* GameIns = GetGameInstance<UBaseGameInstance>();
	if (_PlayState->GetConnectedPlayers() >= UFallConst::MinPlayerCount)
	{
		pNumberOfPlayer = true;
	}
	else
	{
		pNumberOfPlayer = false;
	}
}

// 플레이어 접속시 실행되는 함수 :: 가장 빠름
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
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PreLogin :: 게임 인원이 가득차 접속이 거절되었습니다."));
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PreLogin END ======= "));
}

#pragma region PlayGameMode :: 플레이어 접속시 실행되는 함수 :: PreLogin 다음 (수정 전)
//void APlayGameMode::PostLogin(APlayerController* NewPlayer)
//{
//    Super::PostLogin(NewPlayer);
//
//	// 접속 제한
//	if (true == InvalidConnect)
//	{
//		// 클라이언트를 강제 종료
//		UE_LOG(FALL_DEV_LOG, Error, TEXT("PostLogin :: 게임 인원이 가득차 접속할 수 없습니다."));
//		NewPlayer->ClientTravel(TEXT("/Game/Maps/TitleLevel"), TRAVEL_Absolute);
//		return;
//	}
//
//	// 서버장이 아닐시 리턴
//    if (!HasAuthority()) return;
//
//	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin START ======= "));
//
//    // GameState가 없을시 리턴
//    APlayGameState* FallState = GetGameState<APlayGameState>();
//    if (!FallState) { UE_LOG(FALL_DEV_LOG, Error, TEXT("PostLogin :: GameState가 nullptr 입니다.")); return; }
//
//	// 인원 카운팅
//	FallState->AddConnectedPlayers();
//	int ConnectingPlayer = FallState->GetConnectedPlayers();
//	UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 새로운 플레이어가 접속 했습니다. 현재 접속 인원 : %d"), ConnectingPlayer);
//
//	// 인원 수 체크
//	CheckNumberOfPlayer(FallState);
//	if (true == pNumberOfPlayer ) 
//	{
//		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 게임 플레이를 위한 인원이 충족되었습니다."));
//
//		if (true == UFallConst::UsePlayerLimit) // 인원이 찼고 인원 제한을 사용하는 경우 접속 제한 활성화
//		{
//			InvalidConnect = true;
//			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 인원이 충족되었으므로 접속이 제한됩니다."));
//		}
//	}
//
//	// PlayerState가 없을시 리턴
//    APlayPlayerState* PlayerState = Cast<APlayPlayerState>(NewPlayer->PlayerState);
//    if (!PlayerState)
//    {
//        UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayerState가 nullptr 입니다."));
//        return;
//    }
//
//	// 기존 Player 정보 백업
//	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
//	FString PlayerUniqueID = PlayerState->GetUniqueId()->ToString(); // UniqueID 얻음
//
//	if (GameInstance && GameInstance->IsMovedLevel)
//	{
//		// 스테이지 전환 되었을때 IsDie를 true로 초기화
//		GameInstance->SetIsDie(true);
//
//		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 기존 플레이어 감지. 정보를 로드합니다."));
//
//		FPlayerInfo RestoredInfo;
//		if (GameInstance->InsGetBackedUpPlayerInfo(PlayerUniqueID, RestoredInfo))
//		{
//			RestoredInfo.Status = EPlayerStatus::DEFAULT;  // Status 초기화
//			PlayerState->PlayerInfo = RestoredInfo;
//
//			// 태그 복구
//			NewPlayer->Tags.Add(*RestoredInfo.Tag);
//			// 안전하게 태그 값 가져오기
//			FString TagString = (NewPlayer->Tags.Num() > 0) ? NewPlayer->Tags[0].ToString() : TEXT("태그 없음");
//
//			UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 플레이어 정보 로드 완료 - UniqueID = %s, Tag = %s"),
//				*RestoredInfo.UniqueID, *TagString);
//		}
//	}
//	else
//	{
//		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 신규 플레이어 감지. 정보를 세팅합니다."));
//
//		// 새로운 Player 등록 및 세팅
//		FString UniqueTag = FString::Printf(TEXT("Player%d"), FallState->PlayerInfoArray.Num());
//		PlayerState->SetPlayerInfo(UniqueTag, EPlayerStatus::DEFAULT);
//		
//		// 태그 부여
//		NewPlayer->Tags.Add(*UniqueTag);
//		// 안전하게 태그 값 가져오기
//		FString TagString = (NewPlayer->Tags.Num() > 0) ? NewPlayer->Tags[0].ToString() : TEXT("태그 없음");
//
//		UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 신규 플레이어 정보 세팅 - UniqueID = %s, Tag = %s"),
//			*PlayerState->PlayerInfo.UniqueID, *TagString);
//	}
//
//	// 접속 여부 bool값 true로 변경
//	GameInstance->InsSetbIsConnectedTrue();
//	
//	// 모든 클라이언트에게 정보 동기화
//    SyncPlayerInfo();
//
//	// 게임 인스턴스에 저장된 레벨 이름을 게임 스테이트에 저장
//	FallState->SavePlayLevelName(GameInstance->InsGetCurLevelName());
//	// 게임 인스턴스에 저장된 레벨 에셋 이름을 게임 스테이트에 저장
//	FallState->SavePlayLevelAssetName(GameInstance->InsGetCurLevelAssetName());
//	
//	// 인원 안찼으면 여기서 끝
//	if (false == pNumberOfPlayer) { return; }
//
//	// 카운트 다운 사용할거야?
//	if (true == UFallConst::UseCountDown)
//	{
//		// 카운트 다운 핸들 활성화
//		StartCountdownTimer();
//	}
//	else
//	{
//		// 카운트 다운 바로 종료 처리
//		pCountDownEnd = true;
//	}
//
//	// 카운트 다운도 끝났고, 인원도 찼으니 게임 시작
//	if (true == pCountDownEnd && true == pNumberOfPlayer)
//	{
//		ControllFinishPlayer(FallState);
//		StartGame();
//	}
//
//	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin END ======= "));
//}
#pragma endregion

// 플레이어 접속시 실행되는 함수 :: PreLogin 다음
void APlayGameMode::PostLogin(APlayerController* _NewPlayer)
{
	Super::PostLogin(_NewPlayer);

	// 서버장이 아닐시 리턴
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin START ======= "));

	// GameState가 없을시 리턴
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) { UE_LOG(FALL_DEV_LOG, Error, TEXT("PostLogin :: GameState가 nullptr 입니다.")); return; }

	// 인원 카운팅
	FallState->AddConnectedPlayers();
	int ConnectingPlayer = FallState->GetConnectedPlayers();
	UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 새로운 플레이어가 접속 했습니다. 현재 접속 인원 : %d"), ConnectingPlayer);

	// 인원 수 체크
	CheckNumberOfPlayer(FallState);
	if (true == pNumberOfPlayer)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 게임 플레이를 위한 인원이 충족되었습니다."));

		if (true == UFallConst::UsePlayerLimit) // 인원이 찼고 인원 제한을 사용하는 경우 접속 제한 활성화
		{
			InvalidConnect = true;
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 인원이 충족되었으므로 접속이 제한됩니다."));
		}
	}

	// PlayerState가 없을시 리턴
	APlayPlayerState* PlayerState = Cast<APlayPlayerState>(_NewPlayer->PlayerState);
	if (!PlayerState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayerState가 nullptr 입니다."));
		return;
	}

	// 기존 Player 정보 백업
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	FString PlayerUniqueID = PlayerState->GetUniqueId()->ToString(); // UniqueID 얻음

	if (GameInstance && GameInstance->IsMovedLevel)
	{
		// 스테이지 전환 되었을때 IsDie를 true로 초기화
		GameInstance->SetIsDie(true);

		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 기존 플레이어 감지. 정보를 로드합니다."));

		FPlayerInfo RestoredInfo;
		if (GameInstance->InsGetBackedUpPlayerInfo(PlayerUniqueID, RestoredInfo))
		{
			RestoredInfo.Status = EPlayerStatus::DEFAULT;  // Status 초기화
			PlayerState->PlayerInfo = RestoredInfo;

			// 태그 복구
			_NewPlayer->Tags.Add(*RestoredInfo.Tag);
			// 안전하게 태그 값 가져오기
			FString TagString = (_NewPlayer->Tags.Num() > 0) ? _NewPlayer->Tags[0].ToString() : TEXT("태그 없음");

			UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 플레이어 정보 로드 완료 - UniqueID = %s, Tag = %s"),
				*RestoredInfo.UniqueID, *TagString);
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PostLogin :: 신규 플레이어 감지. 정보를 세팅합니다."));

		// 새로운 Player 등록 및 세팅
		FString UniqueTag = FString::Printf(TEXT("Player%d"), FallState->PlayerInfoArray.Num());
		PlayerState->SetPlayerInfo(UniqueTag, EPlayerStatus::DEFAULT);

		// 태그 부여
		_NewPlayer->Tags.Add(*UniqueTag);
		// 안전하게 태그 값 가져오기
		FString TagString = (_NewPlayer->Tags.Num() > 0) ? _NewPlayer->Tags[0].ToString() : TEXT("태그 없음");

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PostLogin :: 신규 플레이어 정보 세팅 - UniqueID = %s, Tag = %s"),
			*PlayerState->PlayerInfo.UniqueID, *TagString);
	}

	// 접속 여부 bool값 true로 변경
	GameInstance->InsSetbIsConnectedTrue();

	// 모든 클라이언트에게 정보 동기화
	SyncPlayerInfo();

	// 게임 인스턴스에 저장된 레벨 이름을 게임 스테이트에 저장
	FallState->SavePlayLevelName(GameInstance->InsGetCurLevelName());
	// 게임 인스턴스에 저장된 레벨 에셋 이름을 게임 스테이트에 저장
	FallState->SavePlayLevelAssetName(GameInstance->InsGetCurLevelAssetName());

	// 인원 안찼으면 여기서 끝
	if (false == pNumberOfPlayer) { return; }

	// 카운트 다운도 끝났고, 인원도 찼으니 게임 시작
	if (true == pCountDownEnd && true == pNumberOfPlayer)
	{
		ControllFinishPlayer(FallState);
		StartGame();
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode PostLogin END ======= "));
}

void APlayGameMode::HandleStartingNewPlayer_Implementation(APlayerController* _NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(_NewPlayer);

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode HandleStartingNewPlayer START ======= "));

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameMode :: HandleStartingNewPlayer :: GameState가 nullptr 입니다."));
		return;
	}

	// 인원이 충족되지 않았으면 카운트다운을 시작하지 않음
	if (pNumberOfPlayer == false)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: HandleStartingNewPlayer :: 인원 미충족 상태. 카운트다운 시작 보류."));
		return;
	}

	// 카운트다운 조건 확인
	if (UFallConst::UseCountDown == true)
	{
		// 카운트다운 핸들 활성화
		StartCountdownTimer();
	}
	else
	{
		// 카운트다운 없이 바로 종료 처리
		pCountDownEnd = true;
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameMode HandleStartingNewPlayer END ======= "));
}

// 게임 시작
void APlayGameMode::StartGame_Implementation()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 게임이 시작되었습니다."));

	// 캐릭터 움직이게 처리
	SetCharacterMovePossible();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 캐릭터 이동이 가능합니다."));

	// 스테이지 제한 시간 처리
	StartStageLimitTimer();
}

// 캐릭터 이동 가능하게 세팅
void APlayGameMode::SetCharacterMovePossible_Implementation()
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

// 게임 시작 전 카운트다운 핸들 활성화
void APlayGameMode::StartCountdownTimer_Implementation()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: %.0f초 대기후 카운트 다운이 시작됩니다."), UFallConst::FallCountDownStandByTime);

	// 설정한 대기 시간이 끝난 뒤 카운트다운 시작
	FTimerHandle DelayTimer;
	GetWorldTimerManager().SetTimer(DelayTimer, this, &APlayGameMode::StartCountdown, UFallConst::FallCountDownStandByTime, false);
}

// 카운트다운 시작 (대기 후 실행)
void APlayGameMode::StartCountdown()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	float Time = FallState->GetCountDownTime();
	if (!FallState) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 카운트다운 진행 시작. 초기 값: %.0f"), Time);

	if (Time <= 0)
	{
		FallState->SetCountDownTime(10.0f);
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 카운트다운 값이 0이거나 음수라 기본값(10초)으로 설정"));
	}

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &APlayGameMode::UpdateCountdown, 1.0f, true);
}

// 카운트다운 진행 (매초 실행)
void APlayGameMode::UpdateCountdown()
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	FallState->MinusCountDownTime(1.0f);
	float Time = FallState->GetCountDownTime();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 카운트다운 : %.0f"), Time);

	if (FallState->GetCountDownTime() <= 0.0f)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 카운트다운 종료"));

		// 카운트 다운 끝났음을 알림
		pCountDownEnd = true;
		FallState->SetIsCountDownOverTrue();
		StartGame();
	}
}

// 스테이지 제한 시간 타이머 활성화
void APlayGameMode::StartStageLimitTimer_Implementation()
{
	if (!HasAuthority()) return;

	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	if (FallState->GetUseStageLimitTime() == false)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: 스테이지 제한 시간을 사용하지 않으므로 타이머 시작 안함"));
		return;
	}

	float Time = FallState->GetStageLimitTime();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 스테이지 제한 시간 타이머 시작: %.2f초"), Time);

	GetWorldTimerManager().SetTimer(StageLimitTimerHandle, this, &APlayGameMode::OnStageLimitTimeOver, Time, false);
}

// 스테이지 제한 시간 오버 처리
void APlayGameMode::OnStageLimitTimeOver()
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: 제한 시간 초과! 스테이지를 종료합니다."));

	// 예시: 남은 플레이어 FAIL 처리
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	for (APlayerState* PS : FallState->PlayerArray)
	{
		APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
		if (PState && PState->PlayerInfo.Status == EPlayerStatus::DEFAULT)
		{
			PState->PlayerInfo.Status = EPlayerStatus::FAIL;
			UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: Player FAIL 처리됨: %s"), *PState->PlayerInfo.Tag);
		}
	}
	// 다음 맵 이동
	ServerTravelToNextMap(UFallGlobal::GetRandomLevelWithOutPawn());
}

// 목표 골인 인원 수 세팅
void APlayGameMode::SetFinishPlayer(int32 _PlayerCount)
{
	FinishPlayer = _PlayerCount;
}

// 목표 골인 인원 수 제어
void APlayGameMode::ControllFinishPlayer(APlayGameState* _PlayState)
{
	if (!_PlayState) return;

	int32 MinCount = UFallConst::MinPlayerCount;

	switch (_PlayState->CurrentStage)
	{
	case EStageType::STAGE_1:
		if (MinCount <= 2)
		{
			SetFinishPlayer(MinCount);
		}
		else if (MinCount <= 5)
		{
			SetFinishPlayer(3);
		}
		else
		{
			SetFinishPlayer(MinCount / 2);
		}
		break;

	case EStageType::STAGE_2:
		if (MinCount <= 2)
		{
			SetFinishPlayer(1);
		}
		else if (MinCount <= 5)
		{
			SetFinishPlayer(2);
		}
		else
		{
			SetFinishPlayer((MinCount / 2) / 2); 
		}
		break;

	case EStageType::STAGE_3:
		SetFinishPlayer(1);
		break;

	case EStageType::FINISHED:
	default:
		break;
	}
}

// 플랫폼 등록 함수
void APlayGameMode::AddShowDownPlatform(AShowDownPlatform* _Platform)
{
	if (_Platform == nullptr)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: ShowDownPlatform이 nullptr입니다."));
		return;
	}

	AllPlatforms.Add(_Platform);
	UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: 총 ShowDownPlatform 수: %d"), AllPlatforms.Num());
}

AShowDownPlatform* APlayGameMode::GetRandomPlatform()
{
	if (AllPlatforms.Num() == 0)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: GetRandomPlatform :: 플랫폼 배열이 비어있습니다."));
		return nullptr;
	}

	for (int32 Try = 0; Try < 10; ++Try) // 10번 정도 시도
	{
		int32 RandIndex = FMath::RandRange(0, AllPlatforms.Num() - 1);
		AShowDownPlatform* Candidate = AllPlatforms[RandIndex];

		if (Candidate && Candidate->IsLive)
		{
			Candidate->IsLive = false; // 중복 방지
			UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: GetRandomPlatform :: 선택된 플랫폼 인덱스: %d"), RandIndex);
			return Candidate;
		}
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameMode :: GetRandomPlatform :: 사용 가능한 플랫폼이 없습니다."));
	return nullptr;
}
