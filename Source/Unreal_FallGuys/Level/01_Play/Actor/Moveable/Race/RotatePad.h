// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatePad")
	float DelayTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatePad");
	float RotateSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatePad");
	float ReCoverSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "RotatePad")
	float LimitAngle = -65.0f;

private:
	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();

	UFUNCTION(BlueprintCallable)
	bool CheckMoveUp();

	UFUNCTION(BlueprintCallable)
	bool CheckMoveDown();
};
