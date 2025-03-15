// Fill out your copyright notice in the Description page of Project Settings.

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
    APlayerController* PlayerController;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FPlayerInfo PlayerInfo;

    FPlayerInfoEntry() : PlayerController(nullptr), PlayerInfo() {}

    FPlayerInfoEntry(APlayerController* InController, const FPlayerInfo& InInfo)
        : PlayerController(InController), PlayerInfo(InInfo) {
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
    UFUNCTION(BlueprintCallable, Category = "PLAYER LIST")
    void SyncPlayerInfoFromPlayerState();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};