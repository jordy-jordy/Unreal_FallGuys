// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include <Global/GlobalEnum.h>
#include <Mode/01_Play/PlayEnum.h>

#include "PlayPlayerState.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInfo
{
    GENERATED_BODY()

    // 플레이어의 고유 ID 저장
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString UniqueID;

    // Player0, Player1 같은 태그
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString Tag;

    // 플레이어 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EPlayerStatus Status;

    // 팀 정보
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EPlayerTeam Team = EPlayerTeam::None;

    FPlayerInfo()
        : Tag(TEXT("NoTag")), Status(EPlayerStatus::DEFAULT), Team(EPlayerTeam::None)
    {
    }
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API APlayPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    APlayPlayerState();

    // 개별 플레이어 정보
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "PLAYER INFO")
    FPlayerInfo PlayerInfo;
    
    // 플레이어 정보 설정
    UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYER INFO")
    void SetPlayerInfo(const FString& _Tag, EPlayerStatus _Status);
    void SetPlayerInfo_Implementation(const FString& _Tag, EPlayerStatus _Status);

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

