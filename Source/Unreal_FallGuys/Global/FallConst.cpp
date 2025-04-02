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

// 기본 로컬 아이피
FString UFallConst::DefaultIP = TEXT("127.0.0.1");

// 기본 로컬 포트
FString UFallConst::DefaultPort = TEXT("30000");


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	게임 시작 세팅 관련 함수들			
////	게임 시작을 위해선 ** 최소 인원(중요) **을 충족해야 합니다
////	최소 인원(MinPlayerCount)을 1로 두면 서버장 혼자 접속해도 게임을 시작할 수 있습니다.
////	접속 제한 기능(UsePlayerLimit)은 최소 인원이 모두 찼을 경우 타이틀 화면에서 더이상 접속하지 못하도록 막는 기능입니다.
////	카운트 다운(UseCountDown)은 카운트 다운이 끝난 뒤 게임을 시작하도록 할지 결정하는 기능입니다.
////	false로 하면 카운트 다운 없이 바로 게임이 시작됩니다.
////	다만 ** 최소인원 ** 은 무조건 채워야 시작되는 것을 잊지 마세요!!
////	카운트 다운은 바로 시작되는 것이 아닌, 대기 시간(FallCountDownStandByTime)이 끝난 다음에 실행됩니다.
////	FallCountDownTime 으로 카운트 다운의 초를 조절하세요.

// 게임 시작을 위한 최소 플레이어 수
int UFallConst::MinPlayerCount = 2;

// 접속 제한 기능을 사용할 것인지 결정하는 변수
bool UFallConst::UsePlayerLimit = true;

// 카운트 다운을 사용할 것인지 결정하는 변수
bool UFallConst::UseCountDown = false;

// 카운트 다운 대기 시간
float UFallConst::FallCountDownStandByTime = 1.0f;

// 카운트 다운 시간
float UFallConst::FallCountDownTime = 5.0f;

// 몇 초 뒤에 레벨 시네마틱을 보여줄지 결정하는 시간
float UFallConst::LevelCinematicReady = 5.0f;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 각 달걀의 점수
int32 UFallConst::EggScore = 20;

// Stage 플레이 제한 시간 - 개발 중일때만 씀
float UFallConst::FallStageLimitTime = 10.0f;

FName UFallConst::UI::TitleMainPanelName = TEXT("TitleMain");
FName UFallConst::UI::PlayMainPanelName = TEXT("PlayMain");
