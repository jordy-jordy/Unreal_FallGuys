  // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Level/01_Play/Components/ObsMovementActorComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UObsMovementActorComponent* ObstacleMoveComp;

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

private:
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
