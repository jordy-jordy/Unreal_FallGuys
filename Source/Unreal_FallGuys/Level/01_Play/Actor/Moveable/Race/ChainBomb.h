// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "ChainBomb.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AChainBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChainBomb();

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

	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ChainBomb")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ChainBomb")
	UStaticMeshComponent* Axis;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ChainBomb")
	UStaticMeshComponent* StartChain;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ChainBomb")
	UStaticMeshComponent* Bomb;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ChainBomb")
	USphereComponent* BombCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChainBomb")
	float PhyTime = 0.0f;

private:
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();
};
