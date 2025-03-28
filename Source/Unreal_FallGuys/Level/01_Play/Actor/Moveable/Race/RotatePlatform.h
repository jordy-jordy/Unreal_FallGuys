// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "Level/01_Play/Components/LaunchActorComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "RotatePlatform.generated.h"


UENUM(BlueprintType)
enum class EPlatformType: uint8
{
	NONE,
	YELLOW,
	PURPLE,
	RED,
	FAN
};

UENUM(BlueprintType)
enum class EStickType : uint8
{
	NONE,
	STRAIGHT,
	CROSS
};


UCLASS()
class UNREAL_FALLGUYS_API ARotatePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatePlatform();

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Launch")
	ULaunchActorComponent* LaunchComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RootScene")
	USceneComponent* RootScene;

	// PlatformMesh	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePlatform")
	UStaticMeshComponent* PlatformAxis;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePlatform")
	UStaticMeshComponent* PlatformBody;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePlatform")
	UCapsuleComponent* StraightCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePlatform")
	UCapsuleComponent* CrossCollision_X;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePlatform")
	UCapsuleComponent* CrossCollision_Y;

	// StickMesh
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotateStick")
	UStaticMeshComponent* StickBody;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatePlatform", meta = (AllowPrivateAccess = "true"))
	EPlatformType PlatformType = EPlatformType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateStick", meta = (AllowPrivateAccess = "true"))
	EStickType StickType = EStickType::NONE;

private:
	// Functions
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OparateMesh();

	UFUNCTION()
	void SetRotateMesh();

#pragma region SetPlatforms
	UFUNCTION()
	void SetPlatform_Y();

	UFUNCTION()
	void SetPlatform_P();

	UFUNCTION()
	void SetPlatform_R();

	UFUNCTION()
	void SetPlatform_F();

	UFUNCTION()
	void SetPlatform();
#pragma endregion

#pragma region SetSticks
	UFUNCTION()
	void SetStick_S();

	UFUNCTION()
	void SetStick_C();

	UFUNCTION()
	void SetStick();
#pragma endregion

};
