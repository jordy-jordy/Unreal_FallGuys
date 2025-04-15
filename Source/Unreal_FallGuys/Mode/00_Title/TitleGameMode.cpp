// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitleGameMode.h"

#include "Unreal_FallGuys.h"
#include "Global/BaseGameInstance.h"


void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 타이틀 모드로 돌아온다 → 플레이 이력 초기화
	ResetGameInsInfo();
}

void ATitleGameMode::Tick(float _Delta)
{
	Super::Tick(_Delta);
}

// 게임 인포 초기화
void ATitleGameMode::ResetGameInsInfo()
{
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(GetGameInstance());
	GameIns->InsResetPlayerAndLevelCondition();
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("TitleGameMode :: BeginPlay :: 플레이 이력 초기화"));
}

