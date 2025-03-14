// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FALLGUYS_API UMovementActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovementActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Variables
	// 'Add_' And 'Limit_' Variables Must be Positive Number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin", meta = (AllowPrivateAccess = "true"))
	bool IsSpinLeft = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spin", meta = (AllowPrivateAccess = "true"))
	FRotator SpinSpeed;

	// Functions
	UFUNCTION()
	void Spin(float DeltaTime, USceneComponent* RootScene);

};
