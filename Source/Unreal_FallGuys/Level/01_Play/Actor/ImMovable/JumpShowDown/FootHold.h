// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "Level/01_Play/Components/TimeEventActorComponent.h"
#include "FootHold.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AFootHold : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFootHold();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TimeEvent")
	UTimeEventActorComponent* TimeEvent;

	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "FootHold")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "FootHold")
	UStaticMeshComponent* Platform;

	// Variables
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "FootHold")
	bool IsStart = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "FootHold")
	float DelayTime = 5.0f;

private:
	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();
};
