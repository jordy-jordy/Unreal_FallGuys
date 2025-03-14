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
    FString Tag; // ���� �±� (��: "Player1")

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EPlayerStatus Status; // ���� �÷��̾� ���� (��: ����, Ż�� ��)

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 Score; // ����

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString Costume; // ������ �ڽ�Ƭ

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

    // ���� �÷��̾� ���� (���� �̵� �Ŀ��� �ڵ� ������)
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Player")
    FPlayerInfo PlayerInfo;

    // �÷��̾��� ���� ID ���� (���������� ������)
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Player")
    FString PlayerUniqueId;

    // �÷��̾� ���� ����
    UFUNCTION(BlueprintCallable, Category = "Player")
    void SetPlayerInfo(const FString& _Tag, EPlayerStatus _Status);

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

