// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerClothManager.generated.h"


USTRUCT(BlueprintType)
struct FPlayerCloth
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Cloth")
    FString ClothID;  // �ڽ�Ƭ�� �ĺ��� ID

    UPROPERTY(BlueprintReadWrite, Category = "Cloth")
    FString MaterialPath;  // ���͸��� ��� (�ʿ��� ���)

    FPlayerCloth()
        : ClothID(TEXT("Default")), MaterialPath(TEXT(""))
    {
    }
};

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayerClothManager : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, Category = "Cloth")
    TMap<int32, FPlayerCloth> PlayerClothMap;  // �÷��̾� ID�� �ڽ�Ƭ ����

    UFUNCTION(BlueprintCallable, Category = "Cloth")
    void SetPlayerCloth(int32 PlayerID, const FPlayerCloth& Cloth);

    UFUNCTION(BlueprintCallable, Category = "Cloth")
    FPlayerCloth GetPlayerCloth(int32 PlayerID) const;
};
