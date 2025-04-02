// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <Global/GlobalEnum.h>

#include "FallConst.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UFallConst : public UObject
{
	GENERATED_BODY()

public:
	// 충돌 프로파일
	class Collision
	{
	public:
		static FName CollisionProfile_Player;
		static FName CollisionProfile_LevelOBJ;
		static FName CollisionProfile_FinalLine;
		static FName CollisionProfile_CheckPoint;
	};

	class UI
	{
	public:
		static FName TitleMainPanelName;
		static FName PlayMainPanelName;
	};

public:
	// 글로벌 데이터 테이블 경로
	static FString GlobalDataTablePath;

	// 플레이 레벨 수동 지정
	static FString PlayLevelName;

	// 게임 시작을 위한 최소 플레이어 수
	static int MinPlayerCount;

	// 기본 로컬 아이피
	static FString DefaultIP;

	// 기본 로컬 포트
	static FString DefaultPort;

	// 카운트 다운 대기 시간
	static float FallCountDownStandByTime;

	// 카운트 다운 시간
	static float FallCountDownTime;

	// 몇 초 뒤에 레벨 시네마틱을 보여줄지 결정하는 시간
	static float LevelCinematicReady;

	// 카운트 다운을 사용할 것인지 결정하는 변수
	static bool UseCountDown;

	// 접속 제한 기능을 사용할 것인지 결정하는 변수
	static bool UsePlayerLimit;

	// Stage 플레이 제한 시간 - 개발 중일때만 씀
	static float FallStageLimitTime;

	// 디폴트 컬러
	static const ECostumeColor DefaultColor = ECostumeColor::DEFAULT;

	// 각 달걀의 점수
	static int32 EggScore;

private:

};
