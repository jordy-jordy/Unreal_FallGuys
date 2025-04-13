// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/01_Play/Components/ObsMovementActorComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "RotatePad.generated.h"

UENUM(BlueprintType)
enum class ERotateState : uint8
{
	DELAY,
	MOVEUP,
	MOVEDOWN
};

UCLASS()
class UNREAL_FALLGUYS_API ARotatePad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatePad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ObstacleMoveComp")
	UObsMovementActorComponent* ObstacleMoveComp;

	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePad")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePad")
	UStaticMeshComponent* Axis;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePad")
	UStaticMeshComponent* Pad;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePad")
	UStaticMeshComponent* Cover;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatePad")
	ERotateState RotateState = ERotateState::DELAY;

private:
	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();
};
