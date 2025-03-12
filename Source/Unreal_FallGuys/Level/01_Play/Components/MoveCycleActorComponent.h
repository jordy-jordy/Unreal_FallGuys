// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveCycleActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FALLGUYS_API UMoveCycleActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveCycleActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Variables
	// LimitVector Must be Positive Number

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveCycle", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TargetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool IsMoveLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	FVector AddLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	FVector LimitMovingVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin", meta = (AllowPrivateAccess = "true"))
	bool IsSpinLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin", meta = (AllowPrivateAccess = "true"))
	FRotator AddRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin", meta = (AllowPrivateAccess = "true"))
	FVector LimitSpinVector;

private:	
	// Funcitons
	UFUNCTION()
	void Move(float DeltaTime);

	UFUNCTION()
	void Spin(float DeltaTime);

	UFUNCTION()
	void SetDirection();

	UFUNCTION()
	void MoveCycle(float DeltaTime);
};
