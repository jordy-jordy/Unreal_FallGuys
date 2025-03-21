// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementActorComponent.generated.h"

UENUM(BlueprintType)
enum class EMoveAxis : uint8
{
	NONE,
	ROLL,
	PITCH,
	YAW
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREAL_FALLGUYS_API UMovementActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovementActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Variables	
	// Move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	FVector MoveSpeed;

	// MoveCycle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MoveCycle", meta = (AllowPrivateAccess = "true"))
	bool IsMoveCycleLeft = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveCycle", meta = (AllowPrivateAccess = "true"))
	FVector MoveCycleSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "MoveCycle", meta = (AllowPrivateAccess = "true"))
	FVector LimitPos;

	// Spin
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin", meta = (AllowPrivateAccess = "true"))
	FRotator SpinSpeed;

	// SpinOnce
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinOnce", meta = (AllowPrivateAccess = "true"))
	FRotator SpinOnceSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "SpinOnce", meta = (AllowPrivateAccess = "true"))
	FRotator StopAngle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "SpinOnce", meta = (AllowPrivateAccess = "true"))
	bool IsDone = false;

	// SpinCycle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpinCycle", meta = (AllowPrivateAccess = "true"))
	bool IsSpinCycleLeft = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinCycle", meta = (AllowPrivateAccess = "true"))
	FRotator SpinCycleSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "SpinCycle", meta = (AllowPrivateAccess = "true"))
	FRotator LimitAngle;

	// Functions
	UFUNCTION()
	void Move_Scene(float DeltaTime, USceneComponent* Target);

	UFUNCTION()
	void Move(float DeltaTime, UStaticMeshComponent* Target);

	UFUNCTION()
	void MoveCycle(float DeltaTime, UStaticMeshComponent* Target, EMoveAxis Axis);

	UFUNCTION()
	void Spin(float DeltaTime, UStaticMeshComponent* Target);

	UFUNCTION()
	void SpinOnce(float DeltaTime, UStaticMeshComponent* Target, EMoveAxis Axis);

	UFUNCTION()
	void SpinCycle(float DeltaTime, UStaticMeshComponent* Target, EMoveAxis Axis);
};
