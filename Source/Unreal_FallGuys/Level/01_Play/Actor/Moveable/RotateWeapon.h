  // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Level/01_Play/Components/MovementActorComponent.h"
#include "RotateWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	HAMMER,
	AXE
};

UCLASS()
class UNREAL_FALLGUYS_API ARotateWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotateWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UMovementActorComponent* MovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RootScene")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Axis")
	UStaticMeshComponent* Axis;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "WeaponCollision")
	UCapsuleComponent* CapsuleCollision;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateWeapon", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType = EWeaponType::HAMMER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	float PhyTime = 0.0f;

private:
	// MeshAddress
	FString RotateAxis_H = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_4_001.SM_obstacle_4_001");
	FString RotateWeapon_H = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_4_002.SM_obstacle_4_002");

	FString RotateAxis_A = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_3_001.SM_obstacle_3_001");
	FString RotateWeapon_A = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_3_002.SM_obstacle_3_002");

	// Functions
	UFUNCTION()
	void OperateMesh();

	UFUNCTION()
	void SetWeaponMesh_H();

	UFUNCTION()
	void SetWeaponMesh_A();
	
	UFUNCTION()
	void SetWeaponMesh();
};
