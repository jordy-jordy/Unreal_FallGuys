// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "ShowDownPlatform.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AShowDownPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShowDownPlatform();

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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Platform")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Platform")
	UStaticMeshComponent* Platform;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Platform")
	UMaterialInterface* Material;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Platform")
	bool IsBlue = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Platform")
	bool IsLive = true;

	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh_Orange();

	UFUNCTION()
	void SetMesh_Blue();
};
