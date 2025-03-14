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
		static FName MainPanelName;
	};

public:
	// 글로벌 데이터 테이블 경로
	static FString GlobalDataTablePath;

	// 플레이 레벨 수동 지정
	static FString PlayLevelName;

	// 게임 시작을 위한 최소 플레이어 수
	static const int MinPlayerCount = 2;

	// 게임 시작이 가능한지 체크
	static bool CanStart;

	// 기본 로컬 아이피
	static FString DefaultIP;

	// 기본 로컬 포트
	static FString DefaultPort;

private:

};
