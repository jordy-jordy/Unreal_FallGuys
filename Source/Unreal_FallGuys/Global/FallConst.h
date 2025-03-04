// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FallConst.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UFallConst : public UObject
{
	GENERATED_BODY()

public:
	class Collision
	{
	public:
		static FName CollisionProfile_Player;
		static FName CollisionProfile_LevelOBJ;
		static FName CollisionProfile_FinalLine;
		static FName CollisionProfile_CheckPoint;
	};

public:
	static FString PlayLevelName;
	static const int MinPlayerCount = 2;

	static FName MenuPanelName;

private:

};
