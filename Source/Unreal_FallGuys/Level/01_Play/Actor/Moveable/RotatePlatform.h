// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "Level/01_Play/Components/LaunchActorComponent.h"
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

	// StickMesh
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotateStick")
	UStaticMeshComponent* StickBody;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatePlatform", meta = (AllowPrivateAccess = "true"))
	EPlatformType PlatformType = EPlatformType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateStick", meta = (AllowPrivateAccess = "true"))
	EStickType StickType = EStickType::NONE;

private:
	// MeshAddress
	// Platform_Y
	FString PlatformAxis_Y = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_14_001.SM_obstacle_14_001");
	FString PlatformBody_Y = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_14_002.SM_obstacle_14_002");

	// Platform_P
	FString PlatformAxis_P = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_15_001.SM_obstacle_15_001");
	FString PlatformBody_P = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_15_002.SM_obstacle_15_002");

	// Platform_R
	FString PlatformAxis_R = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_26_001.SM_obstacle_26_001");
	FString PlatformBody_R = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_26_002.SM_obstacle_26_002");

	// Platform_F
	FString PlatformAxis_F = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_16_001.SM_obstacle_16_001");
	FString PlatformBody_F = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_16_002.SM_obstacle_16_002");

	// Stick
	FString StraightStick = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_17_001.SM_obstacle_17_001");
	FString CrossStick = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_12_001.SM_obstacle_12_001");

	// Functions
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

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
