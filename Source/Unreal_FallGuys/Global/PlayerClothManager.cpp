// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/PlayerClothManager.h"


void UPlayerClothManager::SetPlayerCloth(int32 PlayerID, const FPlayerCloth& Cloth)
{
    PlayerClothMap.Add(PlayerID, Cloth);
}

FPlayerCloth UPlayerClothManager::GetPlayerCloth(int32 PlayerID) const
{
    if (PlayerClothMap.Contains(PlayerID))
    {
        return *PlayerClothMap.Find(PlayerID);
    }
    return FPlayerCloth();  // 기본값 반환
}