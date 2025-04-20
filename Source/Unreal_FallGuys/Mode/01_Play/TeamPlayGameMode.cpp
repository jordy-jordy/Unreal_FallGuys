// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/TeamPlayGameMode.h"
#include "Kismet/GameplayStatics.h" 

#include "Unreal_FallGuys.h"
#include "Global/FallConst.h"
#include "Global/FallGlobal.h"
#include "Global/GlobalEnum.h"
#include "Global/BaseGameInstance.h"
#include "Mode/01_Play/PlayEnum.h"
#include "Mode/01_Play/TeamPlayGameState.h"
#include "Mode/01_Play/PlayPlayerState.h"
#include "Mode/01_Play/PlayPlayerController.h"


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

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= TeamPlayGameMode PostLogin END ======= "));
}

void ATeamPlayGameMode::HandleSeamlessTravelPlayer(AController*& _NewController)
{
	Super::HandleSeamlessTravelPlayer(_NewController);

	// 서버장이 아닐시 리턴
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= TeamPlayGameMode HandleSeamlessTravelPlayer START ======= "));

	APlayerController* NewPlayerController = Cast<APlayerController>(_NewController);
	if (!NewPlayerController) return;

	APlayPlayerState* PlayerState = Cast<APlayPlayerState>(NewPlayerController->PlayerState);
	if (!PlayerState) { UE_LOG(FALL_DEV_LOG, Error, TEXT("TeamPlayGameMode :: PostLogin :: PlayerState가 nullptr입니다.")); return; }

	// 팀 배정
	AssignTeam(PlayerState);

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= TeamPlayGameMode HandleSeamlessTravelPlayer END ======= "));
}


void ATeamPlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) { return; } // 서버에서만 실행
}

void ATeamPlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 서버만 실행
	if (!HasAuthority()) { return; }

	// 게임이 시작되지 않았다면 리턴하도록 해
	if (!bGameStarted) { return; }

	// 팀전이 아니면 여기서 끝
	if (CurLevelInfo_Mode.LevelType != EStageType::TEAM) { return; }

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

	// 배정 받기 전에 동기화
	SyncPlayerInfo();

	// 각 팀 인원 수 계산
	int32 RedCount = 0;
	int32 BlueCount = 0;

	for (const FPlayerInfoEntry& Entry : FallState->PlayerInfoArray)
	{
		if (Entry.PlayerInfo.Team == ETeamType::RED)
		{
			++RedCount;
		}
		else if (Entry.PlayerInfo.Team == ETeamType::BLUE)
		{
			++BlueCount;
		}
	}

	ETeamType AssignedTeam;

	if (FMath::Abs(RedCount - BlueCount) >= 1)
	{
		// 인원 수 차이가 1 이상이면 적은 팀에 배정
		AssignedTeam = (RedCount < BlueCount) ? ETeamType::RED : ETeamType::BLUE;
	}
	else
	{
		// 인원 수 같으면 랜덤 배정
		AssignedTeam = FMath::RandBool() ? ETeamType::RED : ETeamType::BLUE;
	}

	_PlayerState->SetTeam(AssignedTeam);

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: AssignTeam :: 플레이어 %s 팀 배정 완료: %s"),
		*_PlayerState->PlayerInfo.Tag.ToString(),
		*UEnum::GetValueAsString(_PlayerState->PlayerInfo.Team));

	// 배정 받은 후 동기화
	SyncPlayerInfo();
}

// 레드팀 점수 계산
void ATeamPlayGameMode::CountREDTeamScore(int _NumberOfEgg)
{
	if (!HasAuthority()) return;
	
	REDTeamScore = _NumberOfEgg * UFallConst::EggScore;

	// 점수 동기화
	ATeamPlayGameState* TeamState = GWorld->GetGameState<ATeamPlayGameState>();
	TeamState->SetREDTeamScore(REDTeamScore);
}

// 블루팀 점수 계산
void ATeamPlayGameMode::CountBLUETeamScore(int _NumberOfEgg)
{
	if (!HasAuthority()) return;

	BLUETeamScore = _NumberOfEgg * UFallConst::EggScore;

	// 점수 동기화
	ATeamPlayGameState* TeamState = GWorld->GetGameState<ATeamPlayGameState>();
	TeamState->SetBLUETeamScore(BLUETeamScore);
}

// 스테이지 제한 시간 타이머 및 남은 제한 시간 타이머 활성화
void ATeamPlayGameMode::StartStageLimitTimer()
{
	if (!HasAuthority()) return;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: BeginPlay :: 스테이지 제한 시간 타이머 시작: %.2f초"), CurLevelInfo_Mode.StageLimitTime);

	GetWorldTimerManager().SetTimer(StageLimitTimerHandle, this, &ATeamPlayGameMode::OnStageLimitTimeOver, CurLevelInfo_Mode.StageLimitTime, false);
	GetWorldTimerManager().SetTimer(RemainingTimeUpdateHandle, this, &ATeamPlayGameMode::UpdateRemainingTime, 1.0f, true);
}

// 스테이지 남은 제한 시간 타이머 활성화
void ATeamPlayGameMode::UpdateRemainingTime()
{
	float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(StageLimitTimerHandle);
	float TotalTime = CurLevelInfo_Mode.StageLimitTime;
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

	// 게임 종료 설정
	IsEndGame = true;
	ATeamPlayGameState* TeamFallState = GWorld->GetGameState<ATeamPlayGameState>();
	TeamFallState->SetStateIsEndGameTrue();

	// 남은 시간 갱신 중단
	GetWorldTimerManager().ClearTimer(RemainingTimeUpdateHandle);

	// 팀의 승패 결정
	DetermineWinningAndLosingTeams();

	// 공통 종료 로직
	SetEndCondition_Common(TeamFallState);

	// 팀전 종료 로직
	SetEndCondition_Team();

	// 만약 3 스테이지가 아니라면
	if (CurLevelInfo_Mode.CurStagePhase != EStagePhase::STAGE_3)
	{
		// 5초 후 다음 레벨로 서버 트래블
		GetWorldTimerManager().SetTimer(TravelDelayTimerHandle, this, &ATeamPlayGameMode::ServerTravelToNextTeamMap, 10.0f, false);
	}
	else
	{
		// 5초 후 타이틀로 클라이언트 트래블
		GetWorldTimerManager().SetTimer(TravelDelayTimerHandle, this, &ATeamPlayGameMode::ClientTravelToTitleLevel, 10.0f, false);
	}
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
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: DetermineWinningAndLosingTeams :: 레드팀 승리"));
	}
	else if (TEAMRED < TEAMBLUE)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: DetermineWinningAndLosingTeams :: 블루팀 승리"));
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: DetermineWinningAndLosingTeams :: 무승부, 전원 성공 처리"));
	}
}

// 팀전 종료 로직
void ATeamPlayGameMode::SetEndCondition_Team()
{
	// 팀전용 플레이어 정보 백업
	BackUpTeamPlayersInfo();

	// 다음 레벨에 대한 정보 세팅
	SetNextTeamLevelData();
}

// 팀전용 : 플레이어 정보 백업
void ATeamPlayGameMode::BackUpTeamPlayersInfo()
{
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	APlayGameState* PlayGameState = GetGameState<APlayGameState>();

	if (!GameInstance && !PlayGameState) { return; }

	// 백업하기 전에 비워주자
	GameInstance->PlayerInfoBackup.Empty();

	// 현재 게임 상태 가져오기
	for (FPlayerInfoEntry& PlayerEntry : PlayGameState->PlayerInfoArray)
	{
		// 팀 상태 초기화
		PlayerEntry.PlayerInfo.Team = ETeamType::NONE;

		GameInstance->InsBackupPlayerInfo(PlayerEntry.UniqueID, PlayerEntry.PlayerInfo);
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayGameMode :: BackUpPlayersInfo :: 플레이어 정보 백업 완료 - UniqueID = %s, Tag = %s"),
			*PlayerEntry.UniqueID, *PlayerEntry.PlayerInfo.Tag.ToString());
	}
}

// 팀전 다음 레벨의 정보 세팅
void ATeamPlayGameMode::SetNextTeamLevelData()
{
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());

	// 팀전의 경우
	switch (CurLevelInfo_Mode.CurStagePhase)
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

	GetWorld()->ServerTravel(UFallGlobal::GetRandomTeamLevel(), true);
}

// 팀전용 : 로비로 이동 - 클라이언트
void ATeamPlayGameMode::ClientTravelToTitleLevel()
{
	if (!HasAuthority()) { return; }

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: 서버에서 클라이언트들에게 EndLevel 트래블 명령 시작"));

	int32 ControllerIndex = 0;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It, ++ControllerIndex)
	{
		APlayPlayerController* PC = Cast<APlayPlayerController>(It->Get());
		if (PC == nullptr) continue;

		// 0번 컨트롤러는 서버장 → 트래블 제외
		if (ControllerIndex == 0)
		{
			UE_LOG(FALL_DEV_LOG, Log, TEXT("ClientTravelToTitleLevel :: [0번] 서버장 트래블은 클라이언트들이 이동 된 뒤 처리됩니다."));
			continue;
		}

		// 클라이언트 먼저 트래블
		PC->Client_TravelToTitleLevel();
	}

	// 서버장도 엔드 레벨로 가자
	ServerTravelToTitleLevel();
}

// 팀전용 : 로비로 이동 - 서버
void ATeamPlayGameMode::ServerTravelToTitleLevel()
{
	APlayPlayerController* ServerHostPC = Cast<APlayPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (ServerHostPC == nullptr)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("TeamPlayGameMode :: ServerTravelToTitleLevel :: 서버장 컨트롤러를 찾을 수 없습니다."));
		return;
	}

	// 1초 후 서버장 클라이언트 트래블
	FTimerHandle ServerHostTravelHandle;
	GetWorldTimerManager().SetTimer(ServerHostTravelHandle, [ServerHostPC]()
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("TeamPlayGameMode :: 서버장이 1초 뒤 TitleLevel로 ClientTravel 시작"));
			ServerHostPC->ClientTravel(TEXT("/Game/BP/Level/00_Title/TitleLevel"), ETravelType::TRAVEL_Absolute);
		}, 1.0f, false);
}

//LMH
TMap<ETeamType, int> ATeamPlayGameMode::GetTeamFloors()
{
	CalTeam();
	return TeamFloors;
}

