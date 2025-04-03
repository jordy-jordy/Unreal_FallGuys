// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameState.h"
#include <Net/UnrealNetwork.h>

#include <Unreal_FallGuys.h>
#include <Global/FallConst.h>
#include <Global/Data/GlobalDataTable.h>
#include <Global/BaseGameInstance.h>
#include <Level/01_Play/Actor/ImMovable/JumpShowDown/ShowDownStage.h>
#include "Level/01_Play/Actor/EggSpawnManager.h"


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

void APlayGameState::SyncPlayerInfoFromPlayerState_Implementation()
{
	TMap<FString, FPlayerInfo> TempMap;
	for (APlayerState* PlayerState : PlayerArray)
	{
		APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayerState);
		if (PlayPlayerState)
		{
			TempMap.Add(PlayPlayerState->PlayerInfo.UniqueID, PlayPlayerState->PlayerInfo);
		}
	}

	PlayerInfoArray.Empty();
	for (const auto& Elem : TempMap)
	{
		PlayerInfoArray.Add(FPlayerInfoEntry(Elem.Key, Elem.Value));
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameState :: 플레이어 정보 동기화 - UniqueId: %s, Tag: %s"),
			*Elem.Key, *Elem.Value.Tag);
	}
}

// 접속자 수 증가
void APlayGameState::AddConnectedPlayers_Implementation()
{
	ConnectedPlayers++;

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
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("APlayGameState :: 레벨 시네마틱이 종료되었습니다."));
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

// LMH
void APlayGameState::SpawnEggManager()
{
	if (HasAuthority())
	{
		Spawnner = GetWorld()->SpawnActor<AEggSpawnManager>(SpawnManagerFactory, FVector(0, 0, 400.0f), FRotator(0, 0, 0));
		if (Spawnner == nullptr) return;
		Spawnner->SetOwner(this);
	}
}

void APlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameState, PlayerInfoArray);
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
