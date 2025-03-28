// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "RotateSquare.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API ARotateSquare : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotateSquare();

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotateAxis")
	UStaticMeshComponent* RotateAxis;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotateSquare")
	UStaticMeshComponent* RotateSquare;

private:
	// MeshAddress
	FString Axis = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_25_001.SM_obstacle_25_001");
	FString Square = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_25_002.SM_obstacle_25_002");

	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();
};
