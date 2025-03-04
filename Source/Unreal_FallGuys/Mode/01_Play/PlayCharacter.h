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

	FVector GetControllerForward();

	FVector GetControllerRight();

	// 현정 코스튬 변경 테스트용
	UFUNCTION(BlueprintCallable, Category = "Costume")
	void ChangeCostume(FString _CostumeName);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

};
