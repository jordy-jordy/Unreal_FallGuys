// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/01_Play/Components/LaunchActorComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "Bumper.generated.h"


UENUM(BlueprintType)
enum class EBumperType : uint8
{
	PINKPILLAR,
	SCARLETPILLAR,
	TRIBUMPER,
	SQBUMPER
};

UCLASS()
class UNREAL_FALLGUYS_API ABumper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABumper();

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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Bumper")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Bumper")
	UStaticMeshComponent* BumperBody;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bumper", meta = (AllowPrivateAccess = "true"))
	EBumperType BumperType;

private:
	// Function
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OparateMesh();

	UFUNCTION()
	void SetBumperMesh();

};
