// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "MovingPad.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AMovingPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPad();

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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "MovingPad")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "MovingPad")
	UStaticMeshComponent* Pad;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "MovingPad")
	UStaticMeshComponent* Cover;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "MovingPad")
	UBoxComponent* BoxCollision;

	// Variables
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "MovingPad")
	AActor* Player;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "MovingPad")
	bool IsMove = false;

private:
	// MeshAddress
	FString PadCover = TEXT("/Engine/BasicShapes/Cube.Cube");
	FString PadBody = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_9_001.SM_obstacle_9_001");

	// Functions
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();
};
