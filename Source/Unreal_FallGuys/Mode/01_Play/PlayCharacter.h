﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Global/GlobalEnum.h"
#include "Mode/01_Play/PlayEnum.h"
#include "Mode/01_Play/PlayPlayerState.h"
#include "PlayCharacter.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API APlayCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayCharacter();

	UPROPERTY(EditAnywhere, Category = "INPUT", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveForwardAction;
	UPROPERTY(EditAnywhere, Category = "INPUT", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveBackwardAction;
	UPROPERTY(EditAnywhere, Category = "INPUT", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveRightAction;
	UPROPERTY(EditAnywhere, Category = "INPUT", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveLeftAction;

	UFUNCTION(BlueprintCallable)
	void PlayerWMove();

	UFUNCTION(BlueprintCallable)
	void PlayerSMove();

	UFUNCTION(BlueprintCallable)
	void PlayerDMove();

	UFUNCTION(BlueprintCallable)
	void PlayerAMove();

	UFUNCTION(BlueprintCallable)
	void SetCurAnimnation(EPlayerAnimation _Anim)
	{
		CurAnimnation = _Anim;
	}

	UFUNCTION(BlueprintCallable)
	void AddAirTime(float _Deltatime)
	{
		AirTime += _Deltatime;
	}

	UFUNCTION(BlueprintCallable)
	void ResetAirTime()
	{
		AirTime = 0.0f;
	}

	UFUNCTION(BlueprintCallable)
	void DiveSwitch()
	{
		if (true == IsDive)
		{
			IsDive = false;
		}
		else
		{
			IsDive = true;
		}
	}

	UFUNCTION(BlueprintCallable)
	void AddDiveTime(float _Deltatime)
	{
		DiveTime += _Deltatime;
	}

	UFUNCTION(BlueprintCallable)
	void ResetDiveTime()
	{
		DiveTime = 0.0f;
	}

	UFUNCTION(BlueprintCallable)
	void AddGetUpTime(float _Deltatime)
	{
		GetUpTime += _Deltatime;
	}

	UFUNCTION(BlueprintCallable)
	void ResetGetUpTime(float _Deltatime)
	{
		GetUpTime = 0.0f;
	}

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SpectatorOn();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SpectatorOnForRaceOver();

	FVector GetControllerForward();
	FVector GetControllerRight();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerAnimation CurAnimnation = EPlayerAnimation::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AirTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsDive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DiveTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float GetUpTime = 0.0f;

// 김창수 : 닉네임 관련 함수 및 변수
public:
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void S2M_NickName(const FString& _NickName = TEXT(""));
	void S2M_NickName_Implementation(const FString& _NickName = TEXT(""));

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void C2S_NickName(const FString& _NickName = TEXT(""));
	void C2S_NickName_Implementation(const FString& _NickName = TEXT(""));

	UFUNCTION(BlueprintCallable)
	FString GetNickName() const { return NickName; }

private:
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "NickName", meta = (AllowPrivateAccess = "true"))
	FString NickName = TEXT("");


// 이현정 : 코스튬 관련 함수 및 변수
public:
	void SetCharacterCostume(FString _Color, FString _Top, FString _Bot);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void C2S_Costume(const FString& _Color, const FString& _TopName = TEXT(""), const FString& _BotName = TEXT(""));
	void C2S_Costume_Implementation(const FString& _Color, const FString& _TopName = TEXT(""), const FString& _BotName = TEXT(""));

	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void S2M_Costume(const FString& _Color, const FString& _TopName = TEXT(""), const FString& _BotName = TEXT(""));
	void S2M_Costume_Implementation(const FString& _Color, const FString& _TopName = TEXT(""), const FString& _BotName = TEXT(""));

private: 
	UPROPERTY(VisibleAnywhere, Category = "COSTUME")
	UStaticMeshComponent* CostumeTOPStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "COSTUME")
	UStaticMeshComponent* CostumeBOTStaticMesh;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "COSTUME", meta = (AllowPrivateAccess = "true"))
	FString CostumeColor = TEXT("");

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "COSTUME", meta = (AllowPrivateAccess = "true"))
	FString CostumeTopName = TEXT("");

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "COSTUME", meta = (AllowPrivateAccess = "true"))
	FString CostumeBotName = TEXT("");

// 이현정 : 움직임 관련 함수 및 변수
public:
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void S2M_SetCanMoveTrue();
	void S2M_SetCanMoveTrue_Implementation();

	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void S2M_SetCanMoveFalse();
	void S2M_SetCanMoveFalse_Implementation();

private:
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "GAME START", meta = (AllowPrivateAccess = "true"))
	bool CanMove = false;

// 이현정 : 캐릭터 상태 동기화
public:
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerStatus CurStatus = EPlayerStatus::NONE;

	// 디버그용 : 캐릭터 상태 확인
	UFUNCTION(BlueprintCallable)
	void DebugCheckDieStatus();

protected:
	// 서버장의 캐릭터 상태를 세팅
	virtual void PossessedBy(AController* _NewController) override;
	// 클라이언트의 캐릭터 상태를 세팅
	virtual void OnRep_PlayerState() override;

public:
	// 관전자인지 : PlayPlayerState 로 부터 값을 받음
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	bool bIsSpectar = false;
	// 죽었는지 : PlayPlayerState 로 부터 값을 받음
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	bool IsDie = false;
	// 관전자 세팅 했는지 : PlayerGameMode 로 부터 값이 세팅됨
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	bool bSpectatorApplied = false;
	// 투명화 했는지 : PlayerGameMode 로 부터 값이 세팅됨
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	bool bVisibilityApplied = false;
	// 결과 레벨인지
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	bool bIsResultLevel = false;

	// PlayPlayerState 로 부터 정보를 세팅함
	void InitializeFromPlayerInfo(const FPlayerInfo& _Info);

	// 플레이어를 투명화 : PlayGameMode로부터 호출됨 !!! 플레이 도중
	UFUNCTION(NetMulticast, Reliable)
	void S2M_ApplySpectatorVisibilityAtPlay();
	void S2M_ApplySpectatorVisibilityAtPlay_Implementation();

	// 플레이어를 투명화 : 결과 화면에서 - 서버에게 요청
	UFUNCTION(Server, Reliable)
	void C2S_ApplySpectatorVisibilityAtResult();
	void C2S_ApplySpectatorVisibilityAtResult_Implementation();

	// 플레이어를 투명화 : 결과 화면에서 - 서버가 동기화
	UFUNCTION(NetMulticast, Reliable)
	void S2M_ApplySpectatorVisibilityAtResult();
	void S2M_ApplySpectatorVisibilityAtResult_Implementation();

	// 플레이어를 투명화 : PlayGameMode로부터 호출됨 !!! Goal Or Kill 콜리전에 닿았을때
	UFUNCTION(Server, Reliable)
	void C2S_ApplySpectatorVisibilityAtGoalColl();
	void C2S_ApplySpectatorVisibilityAtGoalColl_Implementation();

	// 플레이어를 투명화 : PlayGameMode로부터 호출됨 !!! Goal Or Kill 콜리전에 닿았을때
	UFUNCTION(NetMulticast, Reliable)
	void S2M_ApplySpectatorVisibilityAtGoalColl();
	void S2M_ApplySpectatorVisibilityAtGoalColl_Implementation();

	// 나 관전자 모드냐?
	UFUNCTION(BlueprintCallable)
	bool GetIsSpectar() { return bIsSpectar; }

	// 서버에 랜덤 뷰 요청
	UFUNCTION(Server, Reliable)
	void C2S_RequestRandomView();
	void C2S_RequestRandomView_Implementation();

	// 관전 대상 인덱스
	UPROPERTY()
	int32 SpectateTargetIndex = 0;

	// 인덱스로 서버에 요청
	UFUNCTION(Server, Reliable)
	void C2S_RequestSetViewByIndex(int32 _TargetIndex);
	void C2S_RequestSetViewByIndex_Implementation(int32 _TargetIndex);


protected:
	bool bCallReadySent = false;
	bool bSettedView = false;
	bool bSettedTransparent = false;
	bool bNeedHiddenAtResult = false;

};
