// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/TeamPlayGameMode.h"

#include "Unreal_FallGuys.h"
#include "Global/FallConst.h"
#include "Global/FallGlobal.h"
#include "Global/GlobalEnum.h"
#include "Global/BaseGameInstance.h"
#include "Mode/01_Play/PlayPlayerState.h"
#include "Mode/01_Play/PlayGameState.h"
#include "Mode/01_Play/TeamPlayGameState.h"


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
		*PlayerState->PlayerInfo.Tag,
		*UEnum::GetValueAsString(PlayerState->PlayerInfo.Team));

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SERVER :: ======= TeamPlayGameMode PostLogin END ======= "));
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

//LMH
TMap<ETeamType, int> ATeamPlayGameMode::GetTeamFloors()
{
	CalTeam();
	return TeamFloors;

}

