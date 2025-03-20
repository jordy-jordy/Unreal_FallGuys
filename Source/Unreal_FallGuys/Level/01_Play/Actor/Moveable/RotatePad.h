// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "RotatePad.generated.h"

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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UMovementActorComponent* MovementComponent;

	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePad")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePad")
	UStaticMeshComponent* Axis;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePad")
	UStaticMeshComponent* Pad;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePad")
	UBoxComponent* Cover;

private:
	// MeshAddress
	FString PadCover = TEXT("/Engine/BasicShapes/Cube.Cube");
	FString PadAxis = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_8_001.SM_obstacle_8_001");
	FString PadBody = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_8_002.SM_obstacle_8_002");
};
