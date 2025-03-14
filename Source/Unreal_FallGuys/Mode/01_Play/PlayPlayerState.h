// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include <Global/GlobalEnum.h>

#include "PlayPlayerState.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString Tag; // 고유 태그 (예: "Player1")

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EPlayerStatus Status; // 현재 플레이어 상태 (예: 생존, 탈락 등)

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 Score; // 점수

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString Costume; // 착용한 코스튬

    FPlayerInfo()
        : Tag(TEXT("NoTag")), Status(EPlayerStatus::DEFAULT), Score(0), Costume(TEXT("Default")) {
    }

    FPlayerInfo(const FString& InTag, EPlayerStatus InStatus, int32 InScore, const FString& InCostume)
        : Tag(InTag), Status(InStatus), Score(InScore), Costume(InCostume) {
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

    // 개별 플레이어 정보 (레벨 이동 후에도 자동 유지됨)
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Player")
    FPlayerInfo PlayerInfo;

    // 플레이어의 고유 ID 저장 (서버에서만 유지됨)
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Player")
    FString PlayerUniqueId;

    // 플레이어 정보 설정
    UFUNCTION(BlueprintCallable, Category = "Player")
    void SetPlayerInfo(const FString& _Tag, EPlayerStatus _Status);

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

