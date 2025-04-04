﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameState.h"
#include <Net/UnrealNetwork.h>

#include <Unreal_FallGuys.h>
#include <Global/FallConst.h>
#include <Global/Data/GlobalDataTable.h>
#include <Global/BaseGameInstance.h>
#include <Level/01_Play/Actor/ImMovable/JumpShowDown/ShowDownStage.h>


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
		UBaseGameInstance* GameInstance = GetGameInstance<UBaseGameInstance>();
		if (GameInstance)
		{
			CurrentStage = GameInstance->InsGetSavedStage();
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: BeginPlay :: CurrentStage를 복구했습니다: %s"), *UEnum::GetValueAsString(CurrentStage));
		}

		UseStageLimitTime = SetUseStageLimitTime();
		StageLimitTime = SetStageLimitTime();
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
				*Entry.UniqueID, *Entry.PlayerInfo.Tag);
		}
		else
		{
			PlayerInfoArray[Index] = Entry; // 값 갱신
			UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: SyncPlayerInfo :: 플레이어 정보 갱신 - UID: %s, Tag: %s"),
				*Entry.UniqueID, *Entry.PlayerInfo.Tag);
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

// 접속자 수 증가
void APlayGameState::AddConnectedPlayers_Implementation()
{
	ConnectedPlayers++;

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

	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("PlayGameState :: 접속자 수 갱신 : %d"), ConnectedPlayers);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Message);
	}
}

// Stage 제한 시간 유무 결정 함수
bool APlayGameState::SetUseStageLimitTime() const
{
	const FPlayLevelDataRow* Row = UGlobalDataTable::FindPlayLevelDataByAssetName(GetWorld(), LevelAssetName);
	if (Row)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: 제한 시간을 사용하니? : %s"), Row->UseLimitTime ? TEXT("true") : TEXT("false"));
		return Row->UseLimitTime;
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: 레벨 데이터를 찾을 수 없음. 레벨 이름 : %s"), *LevelAssetName);
	}

	return false;
}

// Stage 제한 시간 결정 함수
float APlayGameState::SetStageLimitTime() const
{
#if UE_BUILD_SHIPPING
	// 패키징 빌드일 경우 데이터 테이블에서 가져오기
	const FPlayLevelDataRow* Row = UGlobalDataTable::FindPlayLevelDataByAssetName(GetWorld(), LevelAssetName);
	if (Row)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: 패키징 빌드 - 레벨 데이터에서 제한 시간 설정: %.2f초"), Row->StageLimitTime);
		return Row->StageLimitTime;
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: 패키징 빌드 - 레벨 데이터를 찾을 수 없음. 기본값 사용"));
	}
#endif

	// 개발 중일 경우 또는 실패했을 때는 Const 값 사용
	UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: 개발 빌드 - Const에서 제한 시간 사용 : %.2f초"), UFallConst::FallStageLimitTime);
	return UFallConst::FallStageLimitTime;
}

// 게임 인스턴스에서 세팅된 레벨 이름 : GameMode에서 호출
void APlayGameState::SavePlayLevelName_Implementation(const FString& _LevelName)
{
	LevelName = _LevelName;
}

// 게임 인스턴스에서 세팅된 레벨 에셋 이름 : GameMode에서 호출
void APlayGameState::SavePlayLevelAssetName_Implementation(const FString& _LevelAssetName)
{
	LevelAssetName = _LevelAssetName;
}

// 레벨 시네마틱 시작을 세팅하는 함수
void APlayGameState::SetCanStartLevelCinematic_Implementation()
{
	CanStartLevelCinematic = true;
}

// 레벨 시네마틱 끝났는지 세팅하는 함수
void APlayGameState::SetIsLevelCinematicEnd_Implementation(bool _Value)
{
	IsLevelCinematicEnd = _Value;
	
	if (_Value == true)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayGameState :: 레벨 시네마틱이 종료되었습니다."));
	}
}

// 골인 목표 인원 수 세팅 : GameMode에서 호출
void APlayGameState::SetGameStateFinishPlayer_Implementation(int _Value)
{
	GameStateFinishPlayer = _Value;
}

// 현재 골인한 플레이어 수 세팅 : GameMode에서 호출
void APlayGameState::SetGameStateCurFinishPlayer_Implementation(int _Value)
{
	GameStateCurFinishPlayer = _Value;
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
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: SetDropOrder :: 실패한 플레이어의 태그(PlayerTag) = %s, 떨어지는 순서(DropOrder) = %d"),
			*Entry.PlayerInfo.Tag, Entry.PlayerInfo.DropOrder);
	}
}

void APlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameState, PlayerInfoArray);
	DOREPLIFETIME(APlayGameState, FailPlayerInfoArray);
	DOREPLIFETIME(APlayGameState, CountDownTime);
	DOREPLIFETIME(APlayGameState, ConnectedPlayers);
	DOREPLIFETIME(APlayGameState, LevelName);
	DOREPLIFETIME(APlayGameState, LevelAssetName);
	DOREPLIFETIME(APlayGameState, IsCountDownOver);
	DOREPLIFETIME(APlayGameState, StageLimitTime);
	DOREPLIFETIME(APlayGameState, UseStageLimitTime);
	DOREPLIFETIME(APlayGameState, CurrentStage);
	DOREPLIFETIME(APlayGameState, CanStartLevelCinematic);
	DOREPLIFETIME(APlayGameState, IsLevelCinematicEnd);
	DOREPLIFETIME(APlayGameState, GameStateFinishPlayer);
	DOREPLIFETIME(APlayGameState, GameStateCurFinishPlayer);
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
			*Info.Tag,
			*UEnum::GetValueAsString(Info.Status),
			Info.DropOrder);
	}
}
