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
	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drown", meta = (AllowPrivateAccess = "true"))
	EDrownType DrownType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drown", meta = (AllowPrivateAccess = "true"))
	bool IsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drown", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed = 5.0f;

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

private:
	UFUNCTION()
	void SetDrown();

	UFUNCTION()
	void SetDirection();

	UFUNCTION()
	void MovePropeller();
};
