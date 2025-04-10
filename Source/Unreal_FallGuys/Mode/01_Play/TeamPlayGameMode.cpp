// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/TeamPlayGameMode.h"

#include "Unreal_FallGuys.h"
#include "Global/FallConst.h"
#include "Global/FallGlobal.h"
#include "Global/GlobalEnum.h"
#include "Global/BaseGameInstance.h"
#include "Mode/01_Play/PlayEnum.h"
#include "Mode/01_Play/TeamPlayGameState.h"
#include "Mode/01_Play/PlayPlayerState.h"


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
	Super::Tick(DeltaSeconds);

	// 서버만 실행
	if (!HasAuthority()) { return; }

	// 게임이 시작되지 않았다면 리턴하도록 해
	if (!bGameStarted) { return; }

	// 팀전이 아니면 여기서 끝
	if (MODE_CurStageType != EStageType::TEAM) { return; }

	// 제한 시간 타이머 시작
	if (bStartedLimitTimer == false)
	{
		StartStageLimitTimer();
		bStartedLimitTimer = true;
	}
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

// 스테이지 제한 시간 타이머 및 남은 제한 시간 타이머 활성화
void ATeamPlayGameMode::StartStageLimitTimer()
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: BeginPlay :: 스테이지 제한 시간 타이머 시작: %.2f초"), MODE_StageLimitTime);

	GetWorldTimerManager().SetTimer(StageLimitTimerHandle, this, &ATeamPlayGameMode::OnStageLimitTimeOver, MODE_StageLimitTime, false);
	GetWorldTimerManager().SetTimer(RemainingTimeUpdateHandle, this, &ATeamPlayGameMode::UpdateRemainingTime, 1.0f, true);
}

// 스테이지 남은 제한 시간 타이머 활성화
void ATeamPlayGameMode::UpdateRemainingTime()
{
	float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(StageLimitTimerHandle);
	float TotalTime = MODE_StageLimitTime;
	float RemainingTime = FMath::Clamp(TotalTime - ElapsedTime, 0.0f, TotalTime);

	ATeamPlayGameState* FallTeamState = GetGameState<ATeamPlayGameState>();
	if (FallTeamState)
	{
		FallTeamState->SetRemainingTime(RemainingTime);
	}
}

// 스테이지 제한 시간 오버 처리
void ATeamPlayGameMode::OnStageLimitTimeOver()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: BeginPlay :: 제한 시간 초과! 스테이지를 종료합니다."));

	// 남은 시간 갱신 중단
	GetWorldTimerManager().ClearTimer(RemainingTimeUpdateHandle);

	// 팀의 승패 결정
	DetermineWinningAndLosingTeams();

	// 공통 종료 로직
	SetEndCondition_Common();

	// 팀전 종료 로직
	SetEndCondition_Team();

	// 5초 후 서버 트래블
	GetWorldTimerManager().SetTimer(TravelDelayTimerHandle, this, &ATeamPlayGameMode::ServerTravelToNextTeamMap, 5.0f, false);
}

// 점수에 따라 승리팀, 패배팀 구분
void ATeamPlayGameMode::DetermineWinningAndLosingTeams()
{
	ATeamPlayGameState* FallTeamState = GetGameState<ATeamPlayGameState>();
	if (!FallTeamState) return;

	int32 TEAMRED = FallTeamState->GetGameStateREDTeamScore();
	int32 TEAMBLUE = FallTeamState->GetGameStateBLUETeamScore();

	for (APlayerState* PlayerState : FallTeamState->PlayerArray)
	{
		APlayPlayerState* PlayPlayerState = Cast<APlayPlayerState>(PlayerState);
		if (!PlayPlayerState) continue;

		ETeamType PlayerTeam = PlayPlayerState->PlayerInfo.Team;

		if (TEAMRED > TEAMBLUE)
		{
			if (PlayerTeam == ETeamType::RED)
			{
				PlayPlayerState->SetPlayerStatus(EPlayerStatus::SUCCESS);
			}
			else if (PlayerTeam == ETeamType::BLUE)
			{
				PlayPlayerState->SetPlayerStatus(EPlayerStatus::FAIL);
			}
		}
		else if (TEAMRED < TEAMBLUE)
		{
			if (PlayerTeam == ETeamType::RED)
			{
				PlayPlayerState->SetPlayerStatus(EPlayerStatus::FAIL);
			}
			else if (PlayerTeam == ETeamType::BLUE)
			{
				PlayPlayerState->SetPlayerStatus(EPlayerStatus::SUCCESS);
			}
		}
		else
		{
			// 무승부는 전부 성공
			PlayPlayerState->SetPlayerStatus(EPlayerStatus::SUCCESS);
		}
	}

	// 동기화
	FallTeamState->SyncPlayerInfoFromPlayerState();

	// 로그
	if (TEAMRED > TEAMBLUE)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("TeamPlayGameMode :: DetermineWinningAndLosingTeams :: 레드팀 승리"));
	}
	else if (TEAMRED < TEAMBLUE)
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("TeamPlayGameMode :: DetermineWinningAndLosingTeams :: 블루팀 승리"));
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Log, TEXT("TeamPlayGameMode :: DetermineWinningAndLosingTeams :: 무승부, 전원 성공 처리"));
	}
}

// 팀전 종료 로직
void ATeamPlayGameMode::SetEndCondition_Team()
{
	// 플레이어 정보 백업
	BackUpPlayersInfo();

	// 다음 레벨에 대한 정보 세팅
	SetNextTeamLevelData();
}

// 팀전 다음 레벨의 정보 세팅
void ATeamPlayGameMode::SetNextTeamLevelData()
{
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());

	// 팀전의 경우
	switch (MODE_CurStagePhase)
	{
	case EStagePhase::STAGE_1:
		GameInstance->InsSetCurStagePhase(EStagePhase::STAGE_2);
		break;

	case EStagePhase::STAGE_2:
		GameInstance->InsSetCurStagePhase(EStagePhase::STAGE_3);
		break;

	case EStagePhase::STAGE_3:
		GameInstance->InsSetCurStagePhase(EStagePhase::FINISHED);
		break;

	default:
		break;
	}

	// 스테이지 전환 했음을 알림
	GameInstance->IsMovedLevel = true;
}

// 다음 팀전 레벨로 이동
void ATeamPlayGameMode::ServerTravelToNextTeamMap()
{
	if (!HasAuthority()) { return; }

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: 서버트래블 감지 ::"));

	GetWorld()->ServerTravel(UFallGlobal::GetRandomTeamLevel(), false);
}

//LMH
TMap<ETeamType, int> ATeamPlayGameMode::GetTeamFloors()
{
	CalTeam();
	return TeamFloors;
}

