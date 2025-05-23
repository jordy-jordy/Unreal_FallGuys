// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/01_Play/Components/ObsMovementActorComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "HorizontalSlider.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AHorizontalSlider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHorizontalSlider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ObstacleMoveComp")
	UObsMovementActorComponent* ObstacleMoveComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RootScene")
	USceneComponent* RootScene;

	// SliderMesh	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "SliderLine")
	UStaticMeshComponent* SliderLine;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Slider")
	UStaticMeshComponent* Slider;

private:
	// Functions
	UFUNCTION()
	void OparateMesh();

	UFUNCTION()
	void SetMesh();
};
