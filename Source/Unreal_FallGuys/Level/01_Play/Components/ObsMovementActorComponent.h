// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObsMovementActorComponent.generated.h"

UENUM(BlueprintType)
enum class EObsAxis : uint8
{
	X, // Roll
	Y, // Pitch
	Z  // Yaw
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FALLGUYS_API UObsMovementActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObsMovementActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveDir");
	bool IsLeft = false;

	// Functions
	bool CompareVector(FVector First, FVector Second, EObsAxis AxisType);
	bool CompareRotator(FRotator First, FRotator Second, EObsAxis AxisType);

	UFUNCTION(BlueprintCallable)
	void Move_Scene(USceneComponent* Target, FVector MoveSpeed, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void Move_Static(UStaticMeshComponent* Target, FVector MoveSpeed, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void MoveOnce(UStaticMeshComponent* Target, FVector MoveSpeed, FVector LimitPos, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void MoveCycle(UStaticMeshComponent* Target, FVector MoveSpeed, FVector LimitPos, EObsAxis Axis, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void Spin(UStaticMeshComponent* Target, FRotator RotateSpeed, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SpinOnce(UStaticMeshComponent* Target, FRotator RotateSpeed, FRotator LimitAngle, float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SpinCycle(UStaticMeshComponent* Target, FRotator RotateSpeed, FRotator LimitAngle, EObsAxis Axis, float DeltaTime);
};
