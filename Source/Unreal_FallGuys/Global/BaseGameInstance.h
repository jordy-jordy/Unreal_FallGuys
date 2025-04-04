﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include <Mode/01_Play/PlayEnum.h>
#include <Mode/01_Play/PlayPlayerState.h>

#include "BaseGameInstance.generated.h"


/**
 *
 */
UCLASS()
class UNREAL_FALLGUYS_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	friend class UGlobalDataTable;
	friend class UFallGlobal;

public:
	UBaseGameInstance();

#pragma region BaseGameInstance :: 서버 관련
protected:
	UFUNCTION(BlueprintCallable, Category = "SERVER")
	void CServerStart(UWorld* _World, FString _Port);

	UFUNCTION(BlueprintCallable, Category = "SERVER")
	void InsSelectedServerStart(UWorld* _World, FString _Port, FString _OpenLevel);

	UFUNCTION(BlueprintCallable, Category = "SERVER")
	void CServerConnect(UWorld* _World, FString _IP, FString _Port);

#pragma endregion

#pragma region BaseGameInstance :: 데이터 관련
protected:
	// 플레이 레벨 데이터 테이블을 얻는 함수
	UFUNCTION(BlueprintCallable, Category = "DATA")
	class UDataTable* GetPlayLevelDataTable() const;

	// 팀플레이 레벨 데이터 테이블을 얻는 함수
	UFUNCTION(BlueprintCallable, Category = "DATA")
	class UDataTable* GetTeamPlayLevelDataTable() const;

private:
	UPROPERTY(VisibleAnywhere, Category = "DATA")
	class UDataTable* DataTables = nullptr;
	class UDataTable* CostumeDataTable = nullptr;
	class UDataTable* CostumeColorDataTable = nullptr;
	class UDataTable* PlayLevelDataTable = nullptr;
	class UDataTable* TeamPlayLevelDataTable = nullptr;
	class UDataTable* ResourceDataTable = nullptr;

#pragma endregion

#pragma region BaseGameInstance :: 코스튬 관련
public: 
	// 저장된 코스튬의 컬러 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	FString InsGetCostumeColor() const { return CostumeColor; }

	// 저장된 코스튬의 상의 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	FString InsGetCostumeTop() const { return CostumeTop; }

	// 저장된 코스튬의 하의 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	FString InsGetCostumeBot() const { return CostumeBot; }

	// 코스튬 컬러 저장
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsSaveCostumeColor(const FString& _CostumeColor);

	// 코스튬 상의 저장
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsSaveCostumeTop(const FString& _CostumeTop);

	// 코스튬 하의 저장
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsSaveCostumeBot(const FString& _CostumeBot);

	// Pawn의 코스튬 컬러 변경
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsChangeCostumeColor(APawn* _Pawn, const FString& _CostumeColor);

	// 코스튬(상, 하의) 공통 함수
	void InsApplyStaticMesh(UStaticMeshComponent* _Comp, UStaticMesh* _Mesh, const FString& _LogContext);

	// Pawn의 코스튬 상의 변경
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsChangeCostumeTop(APawn* _Pawn, UStaticMeshComponent* _UpComp, const FString& _CostumeTop);

	// Pawn의 코스튬 하의 변경
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	void InsChangeCostumeBot(APawn* _Pawn, UStaticMeshComponent* _LowComp, const FString& _CostumeBot);

	// 저장된 코스튬 컬러의 스켈레탈 메시 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	USkeletalMesh* InsGetCostumeColorMesh(APawn* _Pawn, const FString& _MeshName = TEXT("Default"));

	// 코스튬 컬러 UI용 머티리얼 반환 함수
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	UMaterialInterface* InsGetCostumeColorUIMaterial(const FString& _ColorName);

	// 저장된 코스튬의 스테틱 메시 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	UStaticMesh* InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName = TEXT("Default"));

protected:
	// 코스튬 컬러
	UPROPERTY(VisibleAnywhere, Category = "PLAYER COSTUME")
	FString CostumeColor = TEXT("Default");

	// 코스튬 상의
	UPROPERTY(VisibleAnywhere, Category = "PLAYER COSTUME")
	FString CostumeTop = TEXT("Default_Top");

	// 코스튬 하의
	UPROPERTY(VisibleAnywhere, Category = "PLAYER COSTUME")
	FString CostumeBot = TEXT("Default_Bot");

#pragma endregion

#pragma region BaseGameInstance :: 리소스 관련
public: 
	// 리소스의 스테틱 메시 머티리얼 반환
	UFUNCTION(BlueprintCallable, Category = "RESOURCE")
	UMaterialInterface* InsGetResourceMeshMaterial(const FString& _ColorName);

	// 리소스의 스테틱 메시 반환
	UFUNCTION(BlueprintCallable, Category = "RESOURCE")
	UStaticMesh* InsGetResourceMesh(UWorld* _World, const FString& _MeshName);

#pragma endregion

#pragma region BaseGameInstance :: 레벨 관련
public: 
	// Random PlayLevel의 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString InsGetRandomLevel();

	// Random TeamPlayLevel의 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString InsGetRandomTeamLevel();

	// 저장된 레벨의 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString InsGetCurLevelName() const { return CurLevelName; }

	// 저장된 레벨의 에셋 이름 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString InsGetCurLevelAssetName() const { return CurLevelAssetName; }

	// 레벨 가이드 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString InsGetPlayGuideFromAssetName(const FString& _AssetName);

	// 레벨 이미지 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	UTexture2D* InsGetLevelImageFromAssetName(const FString& _AssetName);

	// 플레이 목표 반환
	UFUNCTION(BlueprintCallable, Category = "LEVEL")
	FString InsGetGoalGuideFromAssetName(const FString& _AssetName);

	// 현재 스테이지 저장용 (레벨 전환 시 유지)
	UPROPERTY(BlueprintReadWrite, Category = "LEVEL")
	EStageType SavedStage = EStageType::STAGE_1;

	UFUNCTION(BlueprintCallable)
	void InsSetSavedStage(EStageType _Stage) { SavedStage = _Stage; }

	UFUNCTION(BlueprintCallable)
	EStageType InsGetSavedStage() const { return SavedStage; }

	// 스테이지 끝나고 나오는 결과창인지
	UPROPERTY(BlueprintReadWrite, Category = "LEVEL")
	bool bIsResultLevel = false;

protected:
	// 레벨 이름
	UPROPERTY(VisibleAnywhere, Category = "LEVEL")
	FString CurLevelName = TEXT("테스트");

	// 레벨 에셋 이름
	UPROPERTY(VisibleAnywhere, Category = "LEVEL")
	FString CurLevelAssetName = TEXT("Race2Map");

	// 레벨 이름 리스트 (AssetName 기준)
	UPROPERTY()
	TArray<FString> MapList;

	// 이미 플레이한 맵의 AssetName 목록
	UPROPERTY()
	TSet<FString> PlayedMapSet;

	// 에셋명 → Row 이름 매핑
	UPROPERTY()
	TMap<FString, FString> LevelNameMap;

#pragma endregion

#pragma region BaseGameInstance :: 플레이어 데이터 관련
public: 
	// 플레이어 정보 백업 함수
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	void InsBackupPlayerInfo(const FString& _UniqueID, FPlayerInfo& _PlayerInfo);

	// 백업된 플레이어 정보 가져오기 함수
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	bool InsGetBackedUpPlayerInfo(const FString& _UniqueID, FPlayerInfo& _OutPlayerInfo) const;

	// 저장된 닉네임 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	FString InsGetNickname() const { return Nickname; }

	// 닉네임 변경
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	void InsChangeNickname(const FString& _NewNickname);

	// 서버 연결 상태를 TRUE로 전환
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	void InsSetbIsConnectedTrue() { bIsConnected = true; }

	// 레벨 이동했는지 체크하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PLAYER DATA")
	bool IsMovedLevel = false;

	// 플레이어 상태를 리셋하는 함수
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	void InsResetPlayerCondition() { IsMovedLevel = false; bIsConnected = false; }

	// 닉네임 설정했니?
	UFUNCTION(BlueprintCallable, Category = "PLAYER DATA")
	bool InsGetHasNickname() { return HasNickname; }

	// 플레이어 UniqueID -> FPlayerInfo 매핑 저장소
	TMap<FString, struct FPlayerInfo> PlayerInfoBackup;

protected:
	// 닉네임
	UPROPERTY(VisibleAnywhere, Category = "PLAYER DATA")
	FString Nickname = TEXT("TEST_JORDY");

	// 닉네임 설정 완료
	bool HasNickname = false;

	// 서버 연결 상태 변수
	bool bIsConnected = false;

#pragma endregion

#pragma region BaseGameInstance :: 개발용
public: 
	// 디버그용 : 플레이어 태그 확인
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void InsPrintPlayerInfo();

	// 디버그용 : 현재 접속자 수, 카운트 다운 END 여부, 현재 스테이지 단계, 목표 골인 인원 수 확인
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void InsPrintConnectedPlayers();

	// 디버그용 : 레벨 이름 확인
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void InsPrintLevelName();

	// 디버그용 : 레벨 시네마틱 바로 끝나게
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void InsetLevelCinematicEnd();

	// 디버그용 : 골인 목표 인원, 골인한 인원 출력
	UFUNCTION(BlueprintCallable, Category = "DEBUG")
	void InsGetGameStateCurFinishPlayer();

#pragma endregion
};

