// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "PunchBox.generated.h"

UENUM(BlueprintType)
enum class EPunchState : uint8
{
	DELAY,
	FIRE,
	RECOVER
};

UCLASS()
class UNREAL_FALLGUYS_API APunchBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APunchBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PunchBox")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PunchBox")
	UStaticMeshComponent* PunchBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PunchBox")
	UStaticMeshComponent* Spring;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PunchBox")
	UStaticMeshComponent* Punch;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PunchBox")
	UCapsuleComponent* PunchCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PunchBox")
	UStaticMeshComponent* PillarTop;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PunchBox")
	UStaticMeshComponent* PillarBody;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PunchBox")
	UStaticMeshComponent* PillarBottom;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PunchBox")
	UStaticMeshComponent* Platform;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PunchBox")
	EPunchState PunchState = EPunchState::DELAY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PunchBox")
	float DelayTime = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PunchBox");
	float FireSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PunchBox");
	float RecoverSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PunchBox")
	float LimitAngle = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float PhyTime = 0.0f;

private:
	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetMesh();

	UFUNCTION(BlueprintCallable)
	bool CheckFire();

	UFUNCTION(BlueprintCallable)
	bool CheckRecover();
};
