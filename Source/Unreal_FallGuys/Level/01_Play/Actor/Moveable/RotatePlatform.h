// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatePlatform.generated.h"


UENUM(BlueprintType)
enum class EPlatformType: uint8
{
	NONE,
	YELLOW,
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
	// MeshComponenet
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePlatform")
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePlatform")
	UStaticMeshComponent* RotateAxis;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotatePlatform")
	UStaticMeshComponent* RotatePlatform;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "RotateStick")
	UStaticMeshComponent* RotateStick;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatePlatform", meta = (AllowPrivateAccess = "true"))
	EPlatformType PlatformType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatePlatform", meta = (AllowPrivateAccess = "true"))
	bool IsLeft_Platform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatePlatform", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed_Platform = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateStick", meta = (AllowPrivateAccess = "true"))
	EStickType StickType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateStick", meta = (AllowPrivateAccess = "true"))
	bool IsLeft_Stick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateStick", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed_Stick = 5.0f;

private:
	// MeshAddress
	// RotatePlate
	FString RotateAxis_Y = TEXT("");
	FString RotatePlate_Y = TEXT("");

	FString RotateAxis_R = TEXT("");
	FString RotatePlate_R = TEXT("");

	// RotateStick
	FString RotateStick_S = TEXT("");
	FString RotateStick_C = TEXT("");

	// Functions
	UFUNCTION()
	void SetRotateMesh();
};
