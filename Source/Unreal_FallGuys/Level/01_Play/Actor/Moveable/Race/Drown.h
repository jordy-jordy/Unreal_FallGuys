// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level/01_Play/Components/ObsMovementActorComponent.h"
#include "Global/Data/ResourceDataTable.h"
#include "FallGlobal.h"
#include "Drown.generated.h"

UENUM(BlueprintType)
enum class EDrownType : uint8
{
	BLUE,
	RED,
	MINT
};

UCLASS()
class UNREAL_FALLGUYS_API ADrown : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrown();

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

	// MeshComponenet
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Drown")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Drown")
	UStaticMeshComponent* DrownBody;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Drown")
	UStaticMeshComponent* DrownPropeller_Left;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Drown")
	UStaticMeshComponent* DrownPropeller_Right;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Drown")
	UStaticMeshComponent* DrownLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Drown")
	UStaticMeshComponent* DrownLightArrow;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drown", meta = (AllowPrivateAccess = "true"))
	EDrownType DrownType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drown", meta = (AllowPrivateAccess = "true"))
	bool IsLeft;

private:
	// Functions
	UFUNCTION()
	void SetDrownLocation();

	UFUNCTION()
	void SetDrownRotation();

	UFUNCTION()
	void SetDrownMesh_B();

	UFUNCTION()
	void SetDrownMesh_R();

	UFUNCTION()
	void SetDrownMesh_M();

	UFUNCTION()
	void SetDrownMesh();

	UFUNCTION()
	void SpinPropeller(float DeltaTime);
};
