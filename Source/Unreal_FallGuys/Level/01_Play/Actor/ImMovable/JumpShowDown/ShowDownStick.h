// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "ShowDownStick.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AShowDownStick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShowDownStick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "MovementComponent")
	UMovementActorComponent* MovementComponent;

	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ShowDownStick")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "BigStick")
	UStaticMeshComponent* BigStick;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "SmallStick")
	UStaticMeshComponent* SmallStick;

private:
	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();

	UFUNCTION()
	void AccSticks(float DeltaTime);
};
