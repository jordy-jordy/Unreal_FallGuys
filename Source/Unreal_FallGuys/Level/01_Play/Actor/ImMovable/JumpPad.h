// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/01_Play/Components/LaunchActorComponent.h"
#include "JumpPad.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Launch")
	ULaunchActorComponent* LaunchComponent;

	// MeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "JumpPad")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "JumpPad")
	UStaticMeshComponent* Case;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "JumpPad")
	UStaticMeshComponent* Pad;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "JumpPad")
	UStaticMeshComponent* Light;

private:
	// MeshAddress
	FString PadCase = TEXT("/Game/Platformer_2/Meshes/SM_trampoline_2_001.SM_trampoline_2_001");
	FString JumpPad = TEXT("/Game/Platformer_2/Meshes/SM_trampoline_2_002.SM_trampoline_2_002");
	FString PadLight = TEXT("/Game/Platformer_2/Meshes/SM_trampoline_plasma_002.SM_trampoline_plasma_002");

	// Functions
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OparateMesh();

	UFUNCTION()
	void SetMesh();
};