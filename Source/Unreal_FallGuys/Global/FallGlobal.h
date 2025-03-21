// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"
#include "FallGlobal.generated.h"

/**
 *
 */
UCLASS()
class UNREAL_FALLGUYS_API UFallGlobal : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void AssetPackagePath(UClass* _Class, const FString& _AssetName, FString& _Path);

	UFUNCTION(BlueprintCallable)
	static TArray<FAssetData> AssetsPath(UClass* _Class);

	UFUNCTION(BlueprintCallable)
	static void ServerStart(UObject* _Object, FString _Port);

	UFUNCTION(BlueprintCallable)
	static void SelectedServerStart(UObject* _Object, FString _Port, FString _OpenLevel);

	UFUNCTION(BlueprintCallable)
	static void ServerConnect(UObject* _Object, FString _IP, FString _Port);

	// 저장된 코스튬의 컬러 반환
	UFUNCTION(BlueprintCallable)
	static FString GetCostumeColor(APawn* _Pawn);

	// 저장된 코스튬의 상의 반환
	UFUNCTION(BlueprintCallable)
	static FString GetCostumeTop(APawn* _Pawn);

	// 저장된 코스튬의 하의 반환
	UFUNCTION(BlueprintCallable)
	static FString GetCostumeBot(APawn* _Pawn);

	// Pawn의 코스튬 컬러 변경
	UFUNCTION(BlueprintCallable)
	static void ChangeCostumeColor(APawn* _Pawn, const FString& _CostumeColor);

	// Pawn의 코스튬 상의 변경
	UFUNCTION(BlueprintCallable)
	static void ChangeCostumeTop(APawn* _Pawn, UStaticMeshComponent* _UpComp, const FString& _CostumeTop = TEXT("Default"));

	// Pawn의 코스튬 하의 변경
	UFUNCTION(BlueprintCallable)
	static void ChangeCostumeBot(APawn* _Pawn, UStaticMeshComponent* _LowComp, const FString& _CostumeBot = TEXT("Default"));

	// 저장된 코스튬 컬러의 스켈레탈 메시 반환
	UFUNCTION(BlueprintCallable)
	static USkeletalMesh* GetCostumeColorMesh(APawn* _Pawn, const FString& _MeshName);

	// 저장된 코스튬의 스테틱 메시 반환
	UFUNCTION(BlueprintCallable)
	static UStaticMesh* GetCostumeMesh(APawn* _Pawn, const FString& _MeshName);

	// 리소스의 스테틱 메시 반환
	UFUNCTION(BlueprintCallable)
	static UStaticMesh* GetResourceMesh(APawn* _Pawn, const FString& _MeshName);

	// 닉네임 반환
	UFUNCTION(BlueprintCallable)
	static FString GetNickname(APawn* _Pawn);

	// 닉네임 저장
	UFUNCTION(BlueprintCallable)
	static void ChangeNickname(APawn* _Pawn, const FString& _NewNickname);

	// 플레이 가능한 레벨 반환
	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetAvailableLevels();
	
	// 랜덤 스테이지 반환
	UFUNCTION(BlueprintCallable)
	static FString GetRandomLevel(APawn* _Pawn);

	// 랜덤 레벨 함수에서 얻은 이름 반환
	UFUNCTION(BlueprintCallable)
	static FString GetLevelName();

	// PlayGameMode의 현재 접속한 플레이어 수 반환
	UFUNCTION(BlueprintCallable)
	static int GetConnectedPlayers();

	// 코스튬의
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	static TArray<FString> GetCostumeColorNames(UObject* _WorldContext);

// 이재영 : 메인위젯을 얻는 함수
public:
	UFUNCTION(BlueprintCallable)
	static UTitleMainWidget* GetMainWidget(UWorld* _World);
};
