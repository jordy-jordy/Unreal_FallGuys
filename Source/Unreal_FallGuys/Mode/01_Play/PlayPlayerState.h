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
    FName Tag;
    // 닉네임 :: BaseGameInstance에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString NickName;
    // 플레이어 코스튬 컬러
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString CostumeColor = TEXT("");
    // 플레이어 코스튬 상의
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString CostumeTOP = TEXT("");
    // 플레이어 코스튬 하의
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString CostumeBOT = TEXT("");
    // 플레이어 상태 :: PlayGameMode에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EPlayerStatus Status;
    // 팀 정보 :: TeamPlayGameMode에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    ETeamType Team;
    // 실패했을 경우 떨어지는 순서
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 DropOrder;

    // 플레이어 정보가 모두 동일할 때 TRUE 반환
    bool operator==(const FPlayerInfo& Other) const
    {
        return UniqueID == Other.UniqueID &&
			Tag == Other.Tag &&
			NickName == Other.NickName &&
			CostumeColor == Other.CostumeColor &&
			CostumeTOP == Other.CostumeTOP &&
			CostumeBOT == Other.CostumeBOT &&
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
        : Tag(TEXT("NoTag")), NickName(TEXT("테스트죠르디")), CostumeColor(TEXT("")), CostumeTOP(TEXT("")), CostumeBOT(TEXT("")),
          Status(EPlayerStatus::DEFAULT), Team(ETeamType::NONE), DropOrder(-1)
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
    void BeginPlay() override;

    // 개별 플레이어 정보
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "PLAYER INFO")
    FPlayerInfo PlayerInfo;

    // 클라들에게도 플레이어 인포 동기화
    UFUNCTION(NetMulticast, Reliable)
    void MCAST_ApplyPlayerInfo(const FPlayerInfo& _Info);
    void MCAST_ApplyPlayerInfo_Implementation(const FPlayerInfo& _Info);

    // 플레이어 태그 설정
    UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYER INFO")
    void SetPlayerTag(const FName& _Tag);
    void SetPlayerTag_Implementation(const FName& _Tag);
    
    // 팀 세팅
    UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYER INFO")
    void SetTeam(ETeamType _Team);
    void SetTeam_Implementation(ETeamType _Team);

    // 플레이어 상태 설정 : 게임 끝나기 전
    UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYER INFO")
    void SetPlayerStatus(EPlayerStatus _NewStatus);
    void SetPlayerStatus_Implementation(EPlayerStatus _NewStatus);

    // 플레이어 상태 설정 : 게임 끝난 후 일괄 변경
    UFUNCTION(Reliable, NetMulticast, BlueprintCallable, Category = "PLAYER INFO")
    void SetPlayerStatusOnEnd(EPlayerStatus _NewStatus);
    void SetPlayerStatusOnEnd_Implementation(EPlayerStatus _NewStatus);

    // 플레이어 떨어지는 순서 설정
    UFUNCTION(Reliable, server, BlueprintCallable, Category = "PLAYER INFO")
    void SetPlayerDropOrder(int32 _Order);
    void SetPlayerDropOrder_Implementation(int32 _Order);

    // 닉네임 반환
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    FString GetPlayerStateNickName() { return PlayerInfo.NickName; }

    // 현재 팀 반환
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    ETeamType GetPlayerStateTeam() { return PlayerInfo.Team; }

    // 플레이어 상태 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
	EPlayerStatus GetPlayerStateStatus() { return PlayerInfo.Status; }

    // 승자 표시 함수
    UFUNCTION(Reliable, Server, BlueprintCallable, Category = "PLAYER INFO")
    void SetIsWinner(bool _bWinner);
    void SetIsWinner_Implementation(bool _bWinner);

    // 승자니?
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
	bool GetIsWinner() { return bIsWinner; }

    // 서버 트래블 할 준비
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    void SetbReadyToTravelTrue();

	// 서버 트래블 할 준비 됐어?
	bool GetbReadyToTravel() { return bReadyToTravel; }

protected:
    // 승자 여부
    UPROPERTY(Replicated)
    bool bIsWinner = false;

    // 실시간 상태 동기화를 위한 변수
    UPROPERTY(Replicated)
    EPlayerStatus PlayerStatus;

    // 실시간 떨어지는 순서 동기화를 위한 변수
    UPROPERTY(ReplicatedUsing = OnRep_PlayerDropOrder)
    int32 PlayerDropOrder = -1;

    // 서버트래블 할 준비가 됨
    UPROPERTY(BlueprintReadOnly)
    bool bReadyToTravel = false;

	// PlayerDropOrder 가 변할 때 호출되는 함수 - 동기화
    UFUNCTION()
    void OnRep_PlayerDropOrder();

    // 동기화 관련
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

