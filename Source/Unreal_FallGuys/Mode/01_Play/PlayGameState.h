﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include <Mode/01_Play/PlayPlayerState.h>

#include "PlayGameState.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInfoEntry
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString UniqueID;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FPlayerInfo PlayerInfo;

    FPlayerInfoEntry() : UniqueID(TEXT("")), PlayerInfo() 
    {
    }

    FPlayerInfoEntry(const FString& _InUniqueID, const FPlayerInfo& _InInfo)
        : UniqueID(_InUniqueID), PlayerInfo(_InInfo) 
    {
    }
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API APlayGameState : public AGameState
{
    GENERATED_BODY()

public:
    APlayGameState();

    // 전체 플레이어 정보 목록 (GameState에서 관리)
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "PLAYER LIST")
    TArray<FPlayerInfoEntry> PlayerInfoArray;

    // `PlayerState`에서 데이터 동기화
    UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYER LIST")
    void SyncPlayerInfoFromPlayerState();
    void SyncPlayerInfoFromPlayerState_Implementation();

    // 카운트다운 시간 (서버에서 클라이언트로 동기화됨)
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "GAME PLAY")
    float CountDownTime;

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
};