// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/FallConst.h"


// 충돌 프로파일
FName UFallConst::Collision::CollisionProfile_Player = TEXT("PlayerColl");
FName UFallConst::Collision::CollisionProfile_LevelOBJ = TEXT("LevelObjColl");
FName UFallConst::Collision::CollisionProfile_FinalLine = TEXT("GoalColl");
FName UFallConst::Collision::CollisionProfile_CheckPoint = TEXT("CheckPointColl");

// 플레이 레벨 수동 지정
FString UFallConst::PlayLevelName = TEXT("Race2Map");

// 글로벌 데이터 테이블 경로
FString UFallConst::GlobalDataTablePath = TEXT("/Script/Engine.DataTable'/Game/BP/Global/Data/DT_GlobalDataTable.DT_GlobalDataTable'");

// 게임 시작이 가능한지 체크
bool UFallConst::CanStart = false;

// 기본 로컬 아이피
FString UFallConst::DefaultIP = TEXT("127.0.0.1");

// 기본 로컬 포트
FString UFallConst::DefaultPort = TEXT("30000");

FName UFallConst::UI::MainPanelName = TEXT("TitleMain");
