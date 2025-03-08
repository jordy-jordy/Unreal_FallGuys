// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovableActorComponent.generated.h"

UENUM(BlueprintType)
enum class EAxisType : uint8
{
	Roll UMETA(DisplayName = "X축 회전"),
	Pitch UMETA(DisplayName = "Y축 회전"),
	Yaw UMETA(DisplayName = "Z축 회전"),
};
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_FALLGUYS_API UMovableActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMovableActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LMH", meta = (AllowPrivateAccess = "true"))
	float CurrentPivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LMH", meta = (AllowPrivateAccess = "true"))
	float MaxPivot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LMH", meta = (AllowPrivateAccess = "true"))
	float MinPivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LMH", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LMH", meta = (AllowPrivateAccess = "true"))
	EAxisType AxisType = EAxisType::Yaw;
};
