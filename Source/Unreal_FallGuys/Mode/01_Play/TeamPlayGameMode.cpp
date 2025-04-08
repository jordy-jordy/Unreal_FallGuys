// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/TeamPlayGameMode.h"

#include "Unreal_FallGuys.h"
#include "Global/FallConst.h"
#include "Global/FallGlobal.h"
#include "Global/GlobalEnum.h"
#include "Global/BaseGameInstance.h"
#include "Mode/01_Play/PlayGameState.h"
#include "Mode/01_Play/TeamPlayGameState.h"


ATeamPlayGameMode::ATeamPlayGameMode()
{
	// 기본 게임 스테이트를 팀전용 게임 스테이트로 설정
	GameStateClass = ATeamPlayGameState::StaticClass();
}

void ATeamPlayGameMode::PostLogin(APlayerController* _NewPlayer)
{
	Super::PostLogin(_NewPlayer);

	// 서버장이 아닐시 리턴
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= TeamPlayGameMode PostLogin START ======= "));

	APlayPlayerState* PlayerState = Cast<APlayPlayerState>(_NewPlayer->PlayerState);
	if (!PlayerState) { UE_LOG(FALL_DEV_LOG, Error, TEXT("TeamPlayGameMode :: PostLogin :: PlayerState가 nullptr입니다.")); return; }

	// 팀 배정
	AssignTeam(PlayerState);

	UE_LOG(FALL_DEV_LOG, Log, TEXT("TeamPlayGameMode :: PostLogin :: 플레이어 %s 팀 배정 완료: %s"),
		*PlayerState->PlayerInfo.Tag.ToString(),
		*UEnum::GetValueAsString(PlayerState->PlayerInfo.Team));

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= TeamPlayGameMode PostLogin END ======= "));
}

void ATeamPlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) { return; } // 서버에서만 실행

	// 스테이지 제한 시간을 가져옴
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	MODE_StageLimitTime = GameInstance->InsGetStageLimitTime();
	// 스테이지 제한 시간을 게임 스테이트에 세팅
	ATeamPlayGameState* FallTeamState = GetGameState<ATeamPlayGameState>();
	FallTeamState->SetStageLimitTime(MODE_StageLimitTime);
}

void ATeamPlayGameMode::Tick(float DeltaSeconds)
{
	// 게임이 시작되지 않았으면 리턴
	if (!bGameStarted) { return; } 

	// 스테이지 제한 시간 타이머가 활성화 되었다면 리턴
	if (IsStartedLimitTimer) { return; }

	// 스테이지 제한 시간 처리
	StartStageLimitTimer();
}

// 팀 배정
void ATeamPlayGameMode::AssignTeam(APlayPlayerState* _PlayerState)
{
	APlayGameState* FallState = GetGameState<APlayGameState>();
	if (!FallState) return;

	int32 RedCount = 0;
	int32 BlueCount = 0;

	for (const FPlayerInfoEntry& Entry : FallState->PlayerInfoArray)
	{
		if (Entry.PlayerInfo.Team == ETeamType::RED)
		{
			RedCount++;
		}
		else if (Entry.PlayerInfo.Team == ETeamType::BLUE)
		{
			BlueCount++;
		}
	}

	if (RedCount <= BlueCount)
	{
		_PlayerState->SetTeam(ETeamType::RED);
	}
	else
	{
		_PlayerState->SetTeam(ETeamType::BLUE);
	}
}

// 레드팀 점수 계산
void ATeamPlayGameMode::CountREDTeamScore(int _NumberOfEgg)
{
	REDTeamScore = _NumberOfEgg * UFallConst::EggScore;

	// 점수 동기화
	ATeamPlayGameState* TeamState = GWorld->GetGameState<ATeamPlayGameState>();
	TeamState->SetREDTeamScore(REDTeamScore);
}

// 블루팀 점수 계산
void ATeamPlayGameMode::CountBLUETeamScore(int _NumberOfEgg)
{
	BLUETeamScore = _NumberOfEgg * UFallConst::EggScore;

	// 점수 동기화
	ATeamPlayGameState* TeamState = GWorld->GetGameState<ATeamPlayGameState>();
	TeamState->SetBLUETeamScore(BLUETeamScore);
}

// 스테이지 제한 시간 타이머 활성화
void ATeamPlayGameMode::StartStageLimitTimer()
{
	if (!HasAuthority()) return;

	IsStartedLimitTimer = true;

	ATeamPlayGameState* FallTeamState = GetGameState<ATeamPlayGameState>();
	if (!FallTeamState) return;

	float Time = FallTeamState->GetStageLimitTime();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: BeginPlay :: 스테이지 제한 시간 타이머 시작: %.2f초"), Time);

	GetWorldTimerManager().SetTimer(StageLimitTimerHandle, this, &ATeamPlayGameMode::OnStageLimitTimeOver, Time, false);
}

// 스테이지 제한 시간 오버 처리
void ATeamPlayGameMode::OnStageLimitTimeOver()
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: BeginPlay :: 제한 시간 초과! 스테이지를 종료합니다."));

	// 다음 맵 이동
	ServerTravelToNextMap(UFallGlobal::GetRandomTeamLevel());
}


//LMH
TMap<ETeamType, int> ATeamPlayGameMode::GetTeamFloors()
{
	CalTeam();
	return TeamFloors;
}

