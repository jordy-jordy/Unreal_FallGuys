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

    // 플레이어의 고유 ID :: 언리얼에서 제공하는 고유 ID
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString UniqueID;

    // Player0, Player1 같은 태그 :: PlayGameMode에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString Tag;

    // 닉네임 :: BaseGameInstance에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString NickName;

    // 플레이어 상태 :: PlayGameMode에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EPlayerStatus Status;

    // 팀 정보 :: TeamPlayGameMode에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    ETeamType Team = ETeamType::NONE;

    // 실패했을 경우 떨어지는 순서
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int DropOrder;

    // 플레이어 정보가 모두 동일할 때 TRUE 반환
    bool operator==(const FPlayerInfo& Other) const
    {
        return UniqueID == Other.UniqueID &&
			NickName == Other.NickName &&
            Tag == Other.Tag &&
            Status == Other.Status &&
            Team == Other.Team &&
            DropOrder == Other.DropOrder;
    }
    // 플레이어 정보가 동일하지 않을때 FALSE 반환
    bool operator!=(const FPlayerInfo& Other) const
    {
        return !(*this == Other);
    }

    FPlayerInfo()
        : Tag(TEXT("NoTag")), NickName(TEXT("")), Status(EPlayerStatus::DEFAULT), Team(ETeamType::NONE), DropOrder(-1)
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
    void SetPlayerInfo(const FString& _Tag, const FString& _NickName);
    void SetPlayerInfo_Implementation(const FString& _Tag, const FString& _NickName);
    
    // 팀 세팅
    UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYER INFO")
    void SetTeam(ETeamType _Team);
    void SetTeam_Implementation(ETeamType _Team);

    // 닉네임 반환
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    FString GetPlayerStateNickName() { return PlayerInfo.NickName; }

    // 현재 팀 반환
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    ETeamType GetPlayerStateTeam() { return PlayerInfo.Team; }

    // 플레이어 상태 전환 : 성공
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
	void SetPlayerStatusSuccess() { PlayerInfo.Status = EPlayerStatus::SUCCESS; }

	// 플레이어 상태 전환 : 실패
	UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")  
	void SetPlayerStatusFail() { PlayerInfo.Status = EPlayerStatus::FAIL; }

    // 플레이어 상태 전환 : 디폴트
	UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
	void SetPlayerStatusDefault() { PlayerInfo.Status = EPlayerStatus::DEFAULT; }

    // 플레이어 상태 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
	EPlayerStatus GetPlayerStateStatus() { return PlayerInfo.Status; }

protected:
    // 동기화 관련
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

