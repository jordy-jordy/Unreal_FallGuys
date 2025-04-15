// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitleGameMode.h"

#include "Global/BaseGameInstance.h"


void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(GetGameInstance());
	GameIns->InsResetPlayerAndLevelCondition();
}

void ATitleGameMode::Tick(float _Delta)
{
	Super::Tick(_Delta);
}

