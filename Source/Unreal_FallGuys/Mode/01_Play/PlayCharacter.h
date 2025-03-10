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
	void TestMove(const FVector2D& _Value);

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

	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void S2M_Costume(const FString& _Name);
	void S2M_Costume_Implementation(const FString& _Name);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void C2S_Costume(const FString& _Name);
	void C2S_Costume_Implementation(const FString& _Name);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	// 나 너를 기반으로 생성된 앤데 너 무슨 코스츔이야? 물어보는 함수를 만들 것이다.

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

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "GM", meta = (AllowPrivateAccess = "true"))
	FString CName = TEXT("");
};
