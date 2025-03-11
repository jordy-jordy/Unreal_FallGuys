// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/01_Play/Components/LaunchActorComponent.h"
#include "Pillar.generated.h"

UENUM(BlueprintType)
enum class EPillarType : uint8
{
	PINK,
	SCARLET
};

UCLASS()
class UNREAL_FALLGUYS_API APillar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APillar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pillar")
	ULaunchActorComponent* LaunchComponent;

	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pillar")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pillar")
	UStaticMeshComponent* PillarBody;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pillar", meta = (AllowPrivateAccess = "true"))
	EPillarType PillarType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch", meta = (AllowPrivateAccess = "true"))
	float LaunchForce = 0.0f;

private:
	// MeshAddress
	FString PinkPillar = TEXT("/Game/Platformer_2/Meshes/SM_obstacle_10_001.SM_obstacle_10_001");
	FString ScarletPillar = TEXT("/Game/Platformer_2/Meshes/SM_tower_003.SM_tower_003");

	// Function
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void SetDrownMesh_P();

	UFUNCTION()
	void SetDrownMesh_S();


	UFUNCTION()
	void SetDrownMesh();

};
