// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"
#include "FallGlobal.generated.h"


USTRUCT(BlueprintType)
struct FLevelDisplayInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;  // Row의 Name

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AssetName;  // 레벨의 AssetName
};

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

	// 템플릿 함수: FString 필드 기준으로 Row 검색
	template<typename T>
	static const T* FindRowByFStringField(UDataTable* _Table, const FString& _CompareValue, const FString& _Context, TFunctionRef<FString(const T*)> _FieldGetter)
	{
		if (!_Table) return nullptr;

		TArray<FName> RowNames = _Table->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			const T* Row = _Table->FindRow<T>(RowName, _Context);
			if (Row && _FieldGetter(Row) == _CompareValue)
			{
				return Row;
			}
		}
		return nullptr;
	}

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

	// 코스튬 컬러 UI용 머티리얼 반환
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	static UMaterialInterface* GetCostumeColorUIMaterial(APawn* _Pawn, const FString& _ColorName);

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
	
	// 플레이 가능한 레벨 및 이름 반환
	UFUNCTION(BlueprintCallable)
	static TArray<FLevelDisplayInfo> GetAvailableLevelInfos();

	// 랜덤 스테이지 반환
	UFUNCTION(BlueprintCallable)
	static FString GetRandomLevel(APawn* _Pawn);

	// 레벨 가이드 반환
	UFUNCTION(BlueprintCallable)
	static FString GetPlayGuideFromAssetName(const FString& _AssetName);

	// 레벨 이미지 반환
	UFUNCTION(BlueprintCallable)
	static UTexture2D* GetLevelImageFromAssetName(const FString& _AssetName);

	// PlayGameState : 랜덤 레벨 함수에서 얻은 이름 반환
	UFUNCTION(BlueprintCallable)
	static FString GetLevelName();

	// PlayGameState : 랜덤 레벨 함수에서 얻은 에셋 이름 반환
	UFUNCTION(BlueprintCallable)
	static FString GetLevelAssetName();

	// PlayGameState : 현재 접속한 플레이어 수 반환
	UFUNCTION(BlueprintCallable)
	static int GetConnectedPlayers();

	// PlayGameState : Count Down Time 반환 - 실시간으로 줄어드는 시간
	UFUNCTION(BlueprintCallable)
	static float GetCountDownTime();

	// PlayGameState : Is Count Down Over 반환 - 카운트 다운 끝났어?
	UFUNCTION(BlueprintCallable)
	static bool GetIsCountDownOver();

	// 코스튬 관련 편의 기능 (현재 개발중)
	UFUNCTION(BlueprintCallable, Category = "PLAYER COSTUME")
	static TArray<FString> GetCostumeColorNames(UObject* _WorldContext);


// 이재영 : 메인위젯을 얻는 함수
public:
	UFUNCTION(BlueprintCallable)
	static UTitleMainWidget* GetMainWidget(UWorld* _World);
};
