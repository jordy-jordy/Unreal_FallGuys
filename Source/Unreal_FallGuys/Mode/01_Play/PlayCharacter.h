// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mode/01_Play/PlayEnum.h"
#include "PlayCharacter.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API APlayCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayCharacter();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
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


// 이현정 : 코스튬 관련 함수 및 변수
public:
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void S2M_Costume(const FString& _Color, const FString& _TopName = TEXT(""), const FString& _BotName = TEXT(""));
	void S2M_Costume_Implementation(const FString& _Color, const FString& _TopName = TEXT(""), const FString& _BotName = TEXT(""));

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void C2S_Costume(const FString& _Color, const FString& _TopName = TEXT(""), const FString& _BotName = TEXT(""));
	void C2S_Costume_Implementation(const FString& _Color, const FString& _TopName = TEXT(""), const FString& _BotName = TEXT(""));

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

//LMH
public:
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsDie = true;

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void C2S_IsDie(bool _val);
	void C2S_IsDie_Implementation(bool _val);

// 이현정 : 캐릭터 상태 동기화를 위한 함수
public:
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void S2M_IsDie(bool _val);
	void S2M_IsDie_Implementation(bool _val);
};
