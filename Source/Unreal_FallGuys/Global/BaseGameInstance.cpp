// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include <Global/FallGlobal.h>
#include <Global/FallConst.h>
#include <Global/PlayerClothManager.h>


// ���� ����
void UBaseGameInstance::CServerStart(UWorld* _World, FString _Port)
{
    FString OpenLevel;
    FString LevelPath = TEXT("");

    UFallGlobal::AssetPackagePath(UWorld::StaticClass(), UFallConst::PlayLevelName, LevelPath);
    OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);

    UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
}

// ���� ����
void UBaseGameInstance::CServerConnect(UWorld* _World, FString _IP, FString _Port)
{
    FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *_IP, *_Port);

    UGameplayStatics::OpenLevel(_World, FName(*ConnectLevelName));
}

void UBaseGameInstance::Init()
{
    Super::Init();

    if (!PlayerClothManager)
    {
        PlayerClothManager = NewObject<UPlayerClothManager>();
    }
}

void UBaseGameInstance::SelectPlayerCloth(int32 PlayerID, FString ClothID)
{
    if (!PlayerClothManager) return;

    FPlayerCloth NewCloth;
    NewCloth.ClothID = ClothID;

    if (ClothID == "Costume1")
    {
        NewCloth.MaterialPath = "/Game/Materials/Costume1Material";
    }
    else if (ClothID == "Costume2")
    {
        NewCloth.MaterialPath = "/Game/Materials/Costume2Material";
    }

    PlayerClothManager->SetPlayerCloth(PlayerID, NewCloth);
}
