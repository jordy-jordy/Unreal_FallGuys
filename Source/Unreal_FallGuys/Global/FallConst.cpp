// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/FallConst.h"


// 충돌 프로파일
FName UFallConst::Collision::CollisionProfile_Player = TEXT("PlayerColl");
FName UFallConst::Collision::CollisionProfile_LevelOBJ = TEXT("LevelObjColl");
FName UFallConst::Collision::CollisionProfile_FinalLine = TEXT("GoalColl");
FName UFallConst::Collision::CollisionProfile_CheckPoint = TEXT("CheckPointColl");

// 플레이 레벨 수동 지정
FString UFallConst::PlayLevelName = TEXT("Race2Map");

// 게임 시작을 위한 최소 플레이어 수
int UFallConst::MinPlayerCount = 1;

// 글로벌 데이터 테이블 경로
FString UFallConst::GlobalDataTablePath = TEXT("/Script/Engine.DataTable'/Game/BP/Global/Data/DT_GlobalDataTable.DT_GlobalDataTable'");

// 기본 로컬 아이피
FString UFallConst::DefaultIP = TEXT("127.0.0.1");

// 기본 로컬 포트
FString UFallConst::DefaultPort = TEXT("30000");

// 카운트 다운 시간
float UFallConst::FallCountDownTime = 5.0f;

// 카운트 다운을 사용할 것인지 결정하는 변수
bool UFallConst::UseCountDown = false;

// 접속 제한 기능을 사용할 것인지 결정하는 변수
bool UFallConst::UseMinPlayer = false;

FName UFallConst::UI::TitleMainPanelName = TEXT("TitleMain");
FName UFallConst::UI::PlayMainPanelName = TEXT("PlayMain");
