// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
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
	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Platform")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Platform")
	UStaticMeshComponent* Platform;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Platform")
	UMaterialInterface* Material;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Platform")
	bool IsLive = true;

	float ShakeTime = 5.0f;
	FVector OriginPos = FVector::ZeroVector;

	UFUNCTION()
	void OperateMesh();
};
