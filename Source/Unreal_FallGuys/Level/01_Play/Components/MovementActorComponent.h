// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementActorComponent.generated.h"

UENUM(BlueprintType)
enum class EMoveAxis : uint8
{
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
	// 'Add_' And 'Limit_' Variables Must be Positive Number
	// MoveCycle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveCycle", meta = (AllowPrivateAccess = "true"))
	bool IsMoveCycleLeft = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveCycle", meta = (AllowPrivateAccess = "true"))
	FVector MoveCycleSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "MoveCycle", meta = (AllowPrivateAccess = "true"))
	FVector LimitPos;

	// Spin
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin", meta = (AllowPrivateAccess = "true"))
	bool IsSpinLeft = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin", meta = (AllowPrivateAccess = "true"))
	FRotator SpinSpeed;

	// SpinCycle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinCycle", meta = (AllowPrivateAccess = "true"))
	bool IsSpinCycleLeft = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinCycle", meta = (AllowPrivateAccess = "true"))
	FRotator SpinCycleSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "SpinCycle", meta = (AllowPrivateAccess = "true"))
	FRotator LimitAngle;

	// Functions
	UFUNCTION()
	void MoveCycle(float DeltaTime, UStaticMeshComponent* Target, EMoveAxis Axis);

	UFUNCTION()
	void Spin(float DeltaTime, UStaticMeshComponent* Target);

	UFUNCTION()
	void SpinCycle(float DeltaTime, UStaticMeshComponent* Target, EMoveAxis Axis);
};
