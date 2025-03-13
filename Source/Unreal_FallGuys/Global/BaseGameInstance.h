// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include <Global/Data/GlobalDataTable.h>
#include <Global/GlobalEnum.h>

#include "BaseGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Tag;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EPlayerStatus Status;

	// 명확한 생성자 추가
	FPlayerInfo()
		: Tag(TEXT("NoTag")), Status(EPlayerStatus::DEFAULT) {
	}

	FPlayerInfo(const FString& InTag, EPlayerStatus InStatus)
		: Tag(InTag), Status(InStatus) {
	}
};

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

	// 코스튬 이름 저장
	UFUNCTION(BlueprintCallable, Category = "Costume")
	void InsSaveCostumeName(const FString& _CostumeName);

	// Pawn의 코스튬 변경
	UFUNCTION(BlueprintCallable, Category = "Costume")
	void InsChangeCostume(APawn* _Pawn, const FString& _CostumeName);

	// 저장된 코스튬의 이름 반환
	UFUNCTION(BlueprintCallable, Category = "Costume")
	FString InsGetCostumeName() const
	{
		return CostumeName;
	}

	// 저장된 코스튬의 스켈레탈 메시 반환
	UFUNCTION(BlueprintCallable, Category = "Costume")
	USkeletalMesh* InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName = TEXT("NULL"));

	// 저장된 닉네임 반환
	UFUNCTION(BlueprintCallable, Category = "Name")
	FString InsGetNickname() const
	{
		return Nickname;
	}

	// 닉네임 변경
	UFUNCTION(BlueprintCallable, Category = "Name")
	void InsChangeNickname(const FString& _NewNickname);

	// 랜덤 플레이 레벨의 이름 반환
	UFUNCTION(BlueprintCallable, Category = "Level")
	FString InsGetRandomLevel();

	// 리소스의 스테틱 메시 반환
	UFUNCTION(BlueprintCallable, Category = "Resource")
	UStaticMesh* InsGetResourceMesh(APawn* _Pawn, const FString& _MeshName = TEXT("NULL"));

	// 플레이어 컨트롤러와 정보(태그 + 상태)를 매핑하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	TMap<APlayerController*, FPlayerInfo> PlayerInfoMap;

	// PlayerInfoMap을 백업하기 위한 변수 (레벨 이동 시 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	TMap<FString, FPlayerInfo> PersistentPlayerInfoMap; // 컨트롤러의 이름을 키로 사용

	// 특정 플레이어의 정보 가져오기
	UFUNCTION(BlueprintCallable, Category = "Player")
	FPlayerInfo InsGetPlayerInfo(APlayerController* _PlayerController) const;

	// 특정 플레이어의 정보 설정
	UFUNCTION(BlueprintCallable, Category = "Player")
	void InsSetPlayerInfo(APlayerController* _PlayerController, const FString& _Tag, EPlayerStatus _Status);

	// 모든 플레이어 정보 반환
	UFUNCTION(BlueprintCallable, Category = "Player")
	TMap<APlayerController*, FPlayerInfo> InsGetAllPlayerInfo() const;

	// PlayerInfoMap을 저장 (레벨 이동 시 사용)
	UFUNCTION(BlueprintCallable, Category = "Player")
	void InsSavePlayerInfo();

	// PlayerInfoMap을 복원 (레벨 이동 후)
	UFUNCTION(BlueprintCallable, Category = "Player")
	void InsLoadPlayerInfo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	bool IsMovedLevel = false;

	// 동기화 변수
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	UFUNCTION(BlueprintCallable, Category = "DataTable")
	UDataTable* GetPlayLevelDataTable() const { return PlayLevelDataTable; }


private:
	UFUNCTION(BlueprintCallable, Category = "Server")
	void CServerStart(UWorld* _World, FString _Port);

	UFUNCTION(BlueprintCallable, Category = "Server")
	void InsSelectedServerStart(UWorld* _World, FString _Port, FString _OpenLevel);

	UFUNCTION(BlueprintCallable, Category = "Server")
	void CServerConnect(UWorld* _World, FString _IP, FString _Port);

	UPROPERTY(VisibleAnywhere, Category = "Data")
	class UDataTable* DataTables = nullptr;
	class UDataTable* CostumeDataTable = nullptr;
	class UDataTable* CostumeColorDataTable = nullptr;
	class UDataTable* PlayLevelDataTable = nullptr;
	class UDataTable* ResourceDataTable = nullptr;
	//class UDataTable* ActorDataTable = nullptr;

	// 코스튬 네임
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Costume")
	FString CostumeName = TEXT("NULL");

	// 닉네임
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Name")
	FString Nickname = TEXT("TEST_JORDY");

	// 맵리스트
	TArray<FString> MapList;
	TSet<int> PlayedMapList;

	// 서버 연결 상태 변수
	bool bIsConnected = false;

};

