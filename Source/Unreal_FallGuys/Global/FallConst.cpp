﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/FallConst.h"


FString UFallConst::PlayLevelName = TEXT("Race2Map");  // 초기화

FName UFallConst::Collision::CollisionProfile_Player = TEXT("PlayerColl");
FName UFallConst::Collision::CollisionProfile_LevelOBJ = TEXT("LevelObjColl");
FName UFallConst::Collision::CollisionProfile_FinalLine = TEXT("GoalColl");
FName UFallConst::Collision::CollisionProfile_CheckPoint = TEXT("CheckPointColl");

FName UFallConst::MenuPanelName = TEXT("TitleMenu");
