// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitleGameMode.h"

#include <Unreal_FallGuys.h>
#include <Global/BaseGameInstance.h>


void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();


    UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->InsStartListeningForServers();
        UE_LOG(FALL_DEV_LOG, Log, TEXT("TitleMode: 서버 검색을 시작합니다."));
    }
    else
    {
        UE_LOG(FALL_DEV_LOG, Error, TEXT("TitleMode: GameInstance를 가져올 수 없습니다."));
    }
}

void ATitleGameMode::Tick(float _Delta)
{
	Super::Tick(_Delta);
}

