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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString UniqueID;
    // Player0, Player1 같은 태그 :: PlayGameMode에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName Tag;
    // 닉네임 :: BaseGameInstance에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString NickName;
    // 플레이어 코스튬 컬러
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString CostumeColor = TEXT("");
    // 플레이어 코스튬 상의
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString CostumeTOP = TEXT("");
    // 플레이어 코스튬 하의
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString CostumeBOT = TEXT("");
    // 플레이어 상태 :: PlayGameMode에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EPlayerStatus Status;
    // 팀 정보 :: TeamPlayGameMode에서 지정
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    ETeamType Team;
    // 실패했을 경우 떨어지는 순서
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 DropOrder;
    // 관전자 여부
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsSpectar;
    // 결과화면에서 가려도 되니?
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bCanHiddenAtResult;
    // 랜덤 뷰 타겟
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName SpectateTargetTag;

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
            DropOrder == Other.DropOrder &&
            bIsSpectar == Other.bIsSpectar &&
            bCanHiddenAtResult == Other.bCanHiddenAtResult &&
            SpectateTargetTag == Other.SpectateTargetTag;

    }
    // 플레이어 정보가 동일하지 않을때 FALSE 반환
    bool operator!=(const FPlayerInfo& Other) const
    {
        return !(*this == Other);
    }

    FPlayerInfo()
        : Tag(TEXT("")), NickName(TEXT("")), CostumeColor(TEXT("")), CostumeTOP(TEXT("")), CostumeBOT(TEXT("")),
          Status(EPlayerStatus::DEFAULT), Team(ETeamType::NONE), DropOrder(-1), bIsSpectar(false), bCanHiddenAtResult(false), 
          SpectateTargetTag(TEXT(""))
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

// 중요
public:
    // 개별 플레이어 정보
    UPROPERTY(ReplicatedUsing = OnRep_PlayerInfo, VisibleAnywhere, BlueprintReadOnly, Category = "PLAYER INFO")
    FPlayerInfo PlayerInfo;


// 세팅 함수
public:
    // 서버 트래블 할 준비
    UFUNCTION(Category = "PLAYER INFO")
    void SetControllerReadyToGame();

    // 플레이어 태그 세팅
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void SetPlayerTag(const FName& _Tag);
    void SetPlayerTag_Implementation(const FName& _Tag);

    // 팀 세팅
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void SetTeam(ETeamType _Team);
    void SetTeam_Implementation(ETeamType _Team);

    // 플레이어 상태 세팅 : 게임 끝나기 전
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void SetPlayerStatus(EPlayerStatus _NewStatus);
    void SetPlayerStatus_Implementation(EPlayerStatus _NewStatus);

    // 플레이어 상태 세팅 : 게임 끝난 후 일괄 변경
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void SetPlayerStatusOnEnd(EPlayerStatus _NewStatus);
    void SetPlayerStatusOnEnd_Implementation(EPlayerStatus _NewStatus);

    // 플레이어 떨어지는 순서 세팅
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void SetPlayerDropOrder(int32 _Order);
    void SetPlayerDropOrder_Implementation(int32 _Order);

    // 최종 승자 세팅
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void SetIsWinner(bool _bWinner);
    void SetIsWinner_Implementation(bool _bWinner);

    // 결과 화면인지 세팅 : PlayGameMode에서 호출
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void SetIsResultLevel(bool _Value);
    void SetIsResultLevel_Implementation(bool _Value);

    // 관전자 세팅
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void SetPlayertoSpectar(bool _Value);
    void SetPlayertoSpectar_Implementation(bool _Value);

    // 결과 화면에서 가릴게
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void SetCanHiddenAtResult(bool _Value);
    void SetCanHiddenAtResult_Implementation(bool _Value);

    // 플레이어 인포 동기화
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void S2M_SetPlayInfo(FPlayerInfo _PlayerInfo);
    void S2M_SetPlayInfo_Implementation(FPlayerInfo _PlayerInfo);


// 반환 함수
public:
    // 서버 트래블 할 준비 됐어?
    bool GetControllerReadyToGame() { return bControllerReadyToGame; }

    // 닉네임 반환
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    FString GetPlayerStateNickName() { return PlayerInfo.NickName; }

    // 현재 팀 반환
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    ETeamType GetPlayerStateTeam() { return PlayerInfo.Team; }

    // 플레이어 상태 반환
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    EPlayerStatus GetPlayerStateStatus() { return PlayerInfo.Status; }

    // 승자니?
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
	bool GetIsWinner() { return bIsWinner; }

    // 결과 화면이야?
    UFUNCTION(BlueprintCallable, Category = "GAME INFO")
	bool GetIsResultLevel() { return bIsResultLevel; }

    // 관전자야?
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    bool GetIsSpectar() { return PlayerInfo.bIsSpectar; }

    // 결과 화면에서 가려져도 돼?
    UFUNCTION(BlueprintCallable, Category = "PLAYER INFO")
    bool GetCanHiddenAtResult() { return PlayerInfo.bCanHiddenAtResult; }


// 닉네임, 코스튬 데이터 동기화
public:
    UFUNCTION(Reliable, NetMulticast, Category = "PLAYER INFO")
    void MCAST_ApplyPlayerInfo(const FPlayerInfo& _Info);
    void MCAST_ApplyPlayerInfo_Implementation(const FPlayerInfo& _Info);


protected:
    // 서버트래블 할 준비가 됨
    UPROPERTY()
    bool bControllerReadyToGame = false;
    // 실시간 상태 동기화를 위한 변수
    UPROPERTY(Replicated)
    EPlayerStatus PlayerStatus;
    // 실시간 떨어지는 순서 동기화를 위한 변수
    UPROPERTY(ReplicatedUsing = OnRep_PlayerDropOrder)
    int32 PlayerDropOrder = -1;
    // 승자 여부
    UPROPERTY(Replicated)
    bool bIsWinner = false;
    // 결과화면 여부
    UPROPERTY(Replicated)
    bool bIsResultLevel = false;

    // PlayerInfo 가 변할 때 호출되는 함수 - 동기화
    UFUNCTION()
    void OnRep_PlayerInfo();
	// PlayerDropOrder 가 변할 때 호출되는 함수 - 동기화
    UFUNCTION()
    void OnRep_PlayerDropOrder();
    // 동기화 관련
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

