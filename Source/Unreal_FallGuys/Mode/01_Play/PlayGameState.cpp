// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameState.h"
#include <Net/UnrealNetwork.h>
#include <EngineUtils.h>

#include <Unreal_FallGuys.h>
#include <Global/FallConst.h>
#include <Global/GlobalEnum.h>
#include <Global/Data/GlobalDataTable.h>
#include <Global/BaseGameInstance.h>
#include <Mode/01_Play/PlayGameMode.h>
#include <Mode/01_Play/PlayPlayerController.h>
#include <Mode/01_Play/PlayCharacter.h>


void APlayGameState::RegisterWidgetDelegate(FName _Name, FWidgetDelegate InDelegate)
{
	if (WidgetDelegates.Contains(_Name))
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("GameState :: 위젯 델리게이트에 %s가 이미 있습니다."), *_Name.ToString());
	}
	WidgetDelegates.Add(_Name, InDelegate);
}

void APlayGameState::MCAST_WidgetDelegate_Implementation(FName _Name)
{
	if (WidgetDelegates.Contains(_Name))
	{
		WidgetDelegates[_Name].ExecuteIfBound();
	}
}

APlayGameState::APlayGameState()
{
	CountDownTime = UFallConst::FallCountDownTime;
}

void APlayGameState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameState BeginPlay START ======= "));

	if (HasAuthority())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: BeginPlay :: 스테이지 모드 : %s"), *UEnum::GetValueAsString(CurLevelInfo_GameState.LevelType));
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: BeginPlay :: 스테이지 페이즈 : %s"), *UEnum::GetValueAsString(CurLevelInfo_GameState.CurStagePhase));

		// AlivePlayers를 주기적으로 업데이트
		GetWorldTimerManager().SetTimer(
			AlivePlayersUpdateTimerHandle,
			this,
			&APlayGameState::UpdateAlivePlayers,
			0.5f, // 0.5초마다 갱신
			true
		);
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= PlayGameState BeginPlay END ======= "));
}

// 플레이어 정보 동기화
void APlayGameState::SyncPlayerInfoFromPlayerState_Implementation()
{
	if (!HasAuthority()) return;  // 서버만 실행

	TArray<FPlayerInfoEntry> ChangedEntries;

	for (APlayerState* PlayerState : PlayerArray)
	{
		APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayerState);
		if (!PlayPlayerState) continue;

		const FString& UID = PlayPlayerState->PlayerInfo.UniqueID;
		const FPlayerInfo& Info = PlayPlayerState->PlayerInfo;

		// 이전 정보와 비교
		if (CachedPlayerInfoMap.Contains(UID) && CachedPlayerInfoMap[UID] == Info)
		{
			continue; // 이전과 같다면 스킵
		}

		// 변경 사항 기록
		ChangedEntries.Add(FPlayerInfoEntry(UID, Info));

		// 캐시 갱신
		CachedPlayerInfoMap.Add(UID, Info);
	}

	// 변경된 항목만 PlayerInfoArray에 적용
	for (const FPlayerInfoEntry& Entry : ChangedEntries)
	{
		int32 Index = PlayerInfoArray.IndexOfByPredicate([&](const FPlayerInfoEntry& E)
			{
				return E.UniqueID == Entry.UniqueID;
			});

		if (Index == INDEX_NONE)
		{
			PlayerInfoArray.Add(Entry); // 새로 추가
			UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: SyncPlayerInfo :: 플레이어 정보 추가 - UID: %s, Tag: %s"),
				*Entry.UniqueID, *Entry.PlayerInfo.Tag.ToString());
		}
		else
		{
			PlayerInfoArray[Index] = Entry; // 값 갱신
			UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: SyncPlayerInfo :: 플레이어 정보 갱신 - UID: %s, Tag: %s"),
				*Entry.UniqueID, *Entry.PlayerInfo.Tag.ToString());
		}
	}

	// 유령 UID 제거
	PlayerInfoArray.RemoveAll([&](const FPlayerInfoEntry& Entry)
		{
			return !PlayerArray.ContainsByPredicate([&](APlayerState* PS)
				{
					APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
					return PState && PState->PlayerInfo.UniqueID == Entry.UniqueID;
				});
		});

	// 캐시에서도 제거
	TArray<FString> KeysToRemove;
	for (const auto& Elem : CachedPlayerInfoMap)
	{
		bool bExists = PlayerArray.ContainsByPredicate([&](APlayerState* PS)
			{
				APlayPlayerState* PState = Cast<APlayPlayerState>(PS);
				return PState && PState->PlayerInfo.UniqueID == Elem.Key;
			});

		if (!bExists)
		{
			KeysToRemove.Add(Elem.Key);
		}
	}

	for (const FString& Key : KeysToRemove)
	{
		CachedPlayerInfoMap.Remove(Key);
	}
}

void APlayGameState::UpdateAlivePlayers()
{
	if (!HasAuthority()) return; // 서버에서만 업데이트

	AlivePlayers.Empty();

	if (!bGameStateIsResultLevel) // 일반 스테이지
	{
		for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
		{
			APlayCharacter* PlayerCharacter = *It;
			if (PlayerCharacter)
			{
				APlayPlayerState* PS = PlayerCharacter->GetPlayerState<APlayPlayerState>();
				if (PS && PS->PlayerInfo.Status == EPlayerStatus::DEFAULT)
				{
					AlivePlayers.Add(PlayerCharacter);
				}
			}
		}

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: 일반 스테이지 :: 현재 AlivePlayers 수 : %d"), AlivePlayers.Num());
	}
	else // 결과 화면
	{
		for (TActorIterator<APlayCharacter> It(GetWorld()); It; ++It)
		{
			APlayCharacter* PlayerCharacter = *It;
			if (PlayerCharacter)
			{
				APlayPlayerState* PS = PlayerCharacter->GetPlayerState<APlayPlayerState>();
				if (PS && PS->PlayerInfo.Status == EPlayerStatus::SUCCESS)
				{
					AlivePlayers.Add(PlayerCharacter);
				}
			}
		}

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: 결과 화면 :: 현재 AlivePlayers 수 : %d"), AlivePlayers.Num());
	}
}

// 접속자 수 증가
void APlayGameState::SetConnectedPlayers_Implementation(int _Count)
{
	ConnectedPlayers = _Count;

	// 모든 클라이언트에게 현재 접속자 수를 알려줌
	MulticastUpdateConnectedPlayers(ConnectedPlayers);
}

// 접속자 수 감소
void APlayGameState::MinusConnectedPlayers_Implementation()
{
	ConnectedPlayers--;

	// 모든 클라이언트에게 현재 접속자 수를 알려줌
	MulticastUpdateConnectedPlayers(ConnectedPlayers);
}

// 접속자 수 동기화
void APlayGameState::MulticastUpdateConnectedPlayers_Implementation(int _NewCount)
{
	ConnectedPlayers = _NewCount;

	// 디버그 출력
	UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: MulticastUpdateConnectedPlayers : %d"), ConnectedPlayers);

	if (UFallConst::PrintDebugLog && GEngine)
	{
		FString Message = FString::Printf(TEXT("PlayGameState :: 접속자 수 갱신 : %d"), ConnectedPlayers);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Message);
	}
}

// 레벨 시네마틱 시작하세요
void APlayGameState::SetCanStartLevelCinematic_Implementation()
{
	CanStartLevelCinematic = true;
}

// 레벨 시네마틱 끝났어요
void APlayGameState::SetIsLevelCinematicEnd_Implementation(bool _Value)
{
	IsLevelCinematicEnd = _Value;
	
	APlayGameMode* PlayMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	if (PlayMode)
	{
		PlayMode->SetCinematicEND(IsLevelCinematicEnd);
	}

	if (_Value == true)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: 레벨 시네마틱이 종료되었습니다."));
	}
}

// 골인 목표 인원 수 세팅 완료 했는지
void APlayGameState::SetGameStateGoalCountSetted_Implementation(bool _Value)
{
	bGameStateSettedGoalCount = _Value;
}

// 골인 목표 인원 수 세팅 : PlayGameMode에서 호출
void APlayGameState::SetGameStateFinishPlayer_Implementation(int _Value)
{
	GameStateFinishPlayer = _Value;
}

// 골인한 플레이어 수 세팅 : PlaytGameMode에서 호출
void APlayGameState::SetGameStateCurFinishPlayer_Implementation(int _Count)
{
	GameStateCurFinishPlayer = _Count;
}

// CountDownTime 에서 Value 차감 : PlayGameMode에서 호출
void APlayGameState::MinusCountDownTime_Implementation(float _Value)
{
	CountDownTime -= _Value;
}

// 카운트 다운이 끝났음을 알림 : PlayGameMode에서 호출
void APlayGameState::SetIsCountDownOverTrue_Implementation()
{
	IsCountDownOver = true;
}

// 결과 화면인지 세팅 : PlayGameMode에서 호출
void APlayGameState::SetGameStateIsResultLevel_Implementation(bool _Value)
{
	bGameStateIsResultLevel = _Value;
}

// 게임 시작했음을 세팅 : PlayGameMode에서 호출
void APlayGameState::SetGameStateGameStarted_Implementation(bool _Value)
{
	bGameStateGameStarted = _Value;
}

// 실패한 플레이어 정보 리스트 백업
void APlayGameState::BackUpFailPlayersInfo_Implementation()
{
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());

	if (!GameInstance)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameState :: BackUpFailPlayersInfo :: GameInstance가 nullptr입니다."));
		return;
	}

	// 기존 실패자 백업 초기화
	GameInstance->FailPlayerInfoBackup.Empty();

	for (const FPlayerInfoEntry& Entry : FailPlayerInfoArray)
	{
		GameInstance->FailPlayerInfoBackup.Add(Entry.UniqueID, Entry.PlayerInfo);

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: BackUpFailPlayersInfo :: UID = %s, Tag = %s → FAIL 백업됨"),
			*Entry.UniqueID, *Entry.PlayerInfo.Tag.ToString());
	}
}

// 실패한 유저의 떨어지는 순번을 정해줌
void APlayGameState::SetDropOrder_Implementation()
{
	if (!HasAuthority()) return; // 서버에서만 실행

	FailPlayerInfoArray.Empty();

	TArray<APlayPlayerState*> FailedPlayerStates;

	// 1. 실패한 플레이어만 수집
	for (APlayerState* PlayerState : PlayerArray)
	{
		APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayerState);
		if (PlayPlayerState && PlayPlayerState->PlayerInfo.Status == EPlayerStatus::FAIL)
		{
			FailedPlayerStates.Add(PlayPlayerState);
		}
	}

	const int32 NumFailed = FailedPlayerStates.Num();
	if (NumFailed <= 0)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: SetDropOrder :: 실패한 플레이어가 없습니다."));
		return;
	}

	// 2. 1부터 NumFailed까지 순번 생성
	TArray<int32> DropOrders;
	for (int32 i = 1; i <= NumFailed; ++i)
	{
		DropOrders.Add(i);
	}

	// 3. 셔플
	for (int32 i = DropOrders.Num() - 1; i > 0; --i)
	{
		const int32 j = FMath::RandRange(0, i);
		DropOrders.Swap(i, j);
	}

	// 4. 순번 배정 + FailPlayerInfoArray에 저장
	for (int32 i = 0; i < NumFailed; ++i)
	{
		APlayPlayerState* PlayerState = FailedPlayerStates[i];

		// PlayerState->PlayerInfo.DropOrder = DropOrders[i];
		PlayerState->SetPlayerDropOrder(DropOrders[i]);

		FailPlayerInfoArray.Add(FPlayerInfoEntry(
			PlayerState->PlayerInfo.UniqueID,
			PlayerState->PlayerInfo));
	}

	// 디버그 로그
	for (const FPlayerInfoEntry& Entry : FailPlayerInfoArray)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: SetDropOrder :: 실패한 플레이어의 태그(PlayerTag) = %s, 떨어지는 순서(DropOrder) = %d"),
			*Entry.PlayerInfo.Tag.ToString(), Entry.PlayerInfo.DropOrder);
	}

	// 실패한 유저 정보 GameInstance에 백업
	BackUpFailPlayersInfo();

	// 상태 동기화
	SyncPlayerInfoFromPlayerState();
}

// 실패한 유저 정보 복구
void APlayGameState::RestoreFailPlayersInfo()
{
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayGameState :: RestoreFailPlayersInfo :: GameInstance가 nullptr입니다."));
		return;
	}

	FailPlayerInfoArray.Empty();

	for (const TPair<FString, FPlayerInfo>& Elem : GameInstance->FailPlayerInfoBackup)
	{
		const FString& UID = Elem.Key;
		const FPlayerInfo& Info = Elem.Value;

		FailPlayerInfoArray.Add(FPlayerInfoEntry(UID, Info));

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: RestoreFailPlayersInfo :: UID = %s, Tag = %s → 실패한 유저 정보 복원됨"),
			*UID, *Info.Tag.ToString());
	}
}

// 다음 레벨로 이동 가능혀 : PlayGameMode에 세팅
void APlayGameState::STATESetCanMoveLevel(bool _b)
{
	if (HasAuthority())
	{
		// 서버장이면 직접 세팅
		APlayGameMode* PlayMode = GWorld->GetAuthGameMode<APlayGameMode>();
		PlayMode->SetCanMoveLevel(_b);
	}
	else
	{
		// 클라면 PlayerController를 통해 서버에 요청
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		APlayPlayerController* MyPC = Cast<APlayPlayerController>(PC);
		if (MyPC)
		{
			MyPC->Server_RequestSetCanMoveLevel(_b);
		}
	}
}

// 결과 화면에서 다음 스테이지로 넘어가도록 해 + PlayGameMode에 세팅
void APlayGameState::SetCanMoveResultLevelTrue_Implementation()
{
	bCanMoveResultLevel = true;

	APlayGameMode* PlayMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	if (PlayMode)
	{
		PlayMode->SetCanMoveResultLevel(bCanMoveResultLevel);
	}
}

// 최대 접속자 수 세팅 : PlayGameMode에서 세팅
void APlayGameState::SetStateMaxPlayerCount_Implementation(int _Value)
{
	StateMaxPlayerCount = _Value;
}

// 게임 끝났음 : PlayGameMode에서 세팅
void APlayGameState::SetStateIsEndGameTrue_Implementation()
{
	StateIsEndGame = true;

	// 타이머 클리어
	GetWorldTimerManager().ClearTimer(AlivePlayersUpdateTimerHandle);

	UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: 게임 종료됨 :: AlivePlayers 타이머 클리어"));
}

// 현 스테이지의 골 타입을 반환함
FString APlayGameState::GetSTATEStageGoalType()
{
	if (CurLevelInfo_GameState.EndCondition == EPlayerStatus::SUCCESS)
	{
		return TEXT("RACING");
	}
	else if (CurLevelInfo_GameState.EndCondition == EPlayerStatus::FAIL)
	{
		return TEXT("SURVIVE");
	}
	else
	{
		return TEXT("잘못됨");
	}
}

void APlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameState, PlayerInfoArray);
	DOREPLIFETIME(APlayGameState, FailPlayerInfoArray);
	DOREPLIFETIME(APlayGameState, CountDownTime);
	DOREPLIFETIME(APlayGameState, ConnectedPlayers);
	DOREPLIFETIME(APlayGameState, IsCountDownOver);
	DOREPLIFETIME(APlayGameState, CanStartLevelCinematic);
	DOREPLIFETIME(APlayGameState, IsLevelCinematicEnd);
	DOREPLIFETIME(APlayGameState, GameStateFinishPlayer);
	DOREPLIFETIME(APlayGameState, GameStateCurFinishPlayer);
	DOREPLIFETIME(APlayGameState, bGameStateIsResultLevel);
	DOREPLIFETIME(APlayGameState, bGameStateGameStarted);
	DOREPLIFETIME(APlayGameState, bGameStateSettedGoalCount);
	DOREPLIFETIME(APlayGameState, FailPlayerInfoArray);
	DOREPLIFETIME(APlayGameState, DefaultPlayerInfoArray);
	DOREPLIFETIME(APlayGameState, CurLevelInfo_GameState);
	DOREPLIFETIME(APlayGameState, bCanMoveResultLevel);
	DOREPLIFETIME(APlayGameState, StateMaxPlayerCount);
	DOREPLIFETIME(APlayGameState, STATECanMoveToResultLevel);
	DOREPLIFETIME(APlayGameState, StateIsEndGame);
	DOREPLIFETIME(APlayGameState, AlivePlayers);
}

void APlayGameState::PrintFailPlayersInfo()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("=============== 실패한 플레이어 정보 ==============="));

	if (FailPlayerInfoArray.Num() == 0)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: FailPlayerInfoArray가 비어있습니다."));
		return;
	}

	for (const FPlayerInfoEntry& Entry : FailPlayerInfoArray)
	{
		const FPlayerInfo& Info = Entry.PlayerInfo;

		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: UID: %s | Tag: %s | Status: %s | DropOrder: %d"),
			*Entry.UniqueID,
			*Info.Tag.ToString(),
			*UEnum::GetValueAsString(Info.Status),
			Info.DropOrder);
	}
}

