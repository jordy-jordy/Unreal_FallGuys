// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "Components/BoxComponent.h"
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
	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ShowDownStick")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "BigStick")
	UStaticMeshComponent* BigStick;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "BigStick")
	UBoxComponent* BigStickCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "SmallStick")
	UStaticMeshComponent* SmallStick;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "SmallStick")
	UBoxComponent* SmallStickCollision;


	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BigStick")
	FRotator StartSpeed_Big = FRotator(0.0f, 32.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmallStick")
	FRotator StartSpeed_Small = FRotator (0.0f, -35.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShowDownStick")
	FRotator AccSpeed = FRotator(0.0f, 0.8f, 0.0f);

private:
	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();
};
