// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "RotateRoller.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API ARotateRoller : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotateRoller();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UMovementActorComponent* MovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RootScene")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RollerAxis")
	UStaticMeshComponent* RollerAxis;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RollerBody")
	UStaticMeshComponent* RollerBody;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RollerCollision")
	UCapsuleComponent* CapsuleCollision;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	float PhyTime = 0.0f;

private:
	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();
};