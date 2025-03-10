// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drown", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed = 5.0f;


private:
	// MeshAddress
	// Blue
	FString DrownBody_B = TEXT("/Game/Platformer_2/Meshes/SM_indicator_001.SM_indicator_001");
	FString LeftPropeller_B = TEXT("/Game/Platformer_2/Meshes/SM_indicator_propeller_2_001.SM_indicator_propeller_2_001");
	FString RightPropeller_B = TEXT("/Game/Platformer_2/Meshes/SM_indicator_propeller_1_001.SM_indicator_propeller_1_001");
	FString DrownLight_B = TEXT("/Game/Platformer_2/Meshes/SM_indicator_light_001.SM_indicator_light_001");
	FString LightArrow_B = TEXT("/Game/Platformer_2/Meshes/SM_indicator_arrow_001.SM_indicator_arrow_001");

	// Red
	FString DrownBody_R = TEXT("/Game/Platformer_2/Meshes/SM_drone_002.SM_drone_002");
	FString LeftPropeller_R = TEXT("/Game/Platformer_2/Meshes/SM_drone_propeller_2_002.SM_drone_propeller_2_002");
	FString RightPropeller_R = TEXT("/Game/Platformer_2/Meshes/SM_drone_propeller_1_002.SM_drone_propeller_1_002");
	FString DrownLight_R = TEXT("/Game/Platformer_2/Meshes/SM_indicator_light_002.SM_indicator_light_002");
	FString LightArrow_R = TEXT("/Game/Platformer_2/Meshes/SM_indicator_arrow_002.SM_indicator_arrow_002");

	// Mint
	FString DrownBody_M = TEXT("/Game/Platformer_2/Meshes/SM_indicator_003.SM_indicator_003");
	FString LeftPropeller_M = TEXT("/Game/Platformer_2/Meshes/SM_indicator_propeller_2_003.SM_indicator_propeller_2_003");
	FString RightPropeller_M = TEXT("/Game/Platformer_2/Meshes/SM_indicator_propeller_1_003.SM_indicator_propeller_1_003");
	FString DrownLight_M = TEXT("/Game/Platformer_2/Meshes/SM_indicator_light_003.SM_indicator_light_003");
	FString LightArrow_M = TEXT("/Game/Platformer_2/Meshes/SM_indicator_arrow_003.SM_indicator_arrow_003");

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
	void MovePropeller();
};
