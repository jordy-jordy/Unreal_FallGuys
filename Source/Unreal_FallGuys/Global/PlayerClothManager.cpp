// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/PlayerClothManager.h"


void UPlayerClothManager::SetPlayerCloth(int32 PlayerID, const FPlayerCloth& Cloth)
{
    PlayerClothMap.Emplace(PlayerID, Cloth);
}

FPlayerCloth UPlayerClothManager::GetPlayerCloth(int32 PlayerID) const
{
    return PlayerClothMap.FindRef(PlayerID);
}
