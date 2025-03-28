// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Level/01_Play/Components/TimeEventActorComponent.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "Cannon.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TimeEvent")
	UTimeEventActorComponent* TimeEventComponent;

	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	UStaticMeshComponent* Cannon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	USphereComponent* CannonBodyCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	UCapsuleComponent* CannonLeftEyeCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	UCapsuleComponent* CannonRightEyeCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	UCapsuleComponent* CannonMuzzleCollision;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	FVector SpawnPos = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	float RespawnTime = 0.0f;

private:
	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();
};
