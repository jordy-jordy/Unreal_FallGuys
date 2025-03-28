// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaunchActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FALLGUYS_API ULaunchActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULaunchActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch", meta = (AllowPrivateAccess = "true"))
	float LaunchForce = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch", meta = (AllowPrivateAccess = "true"))
	float JumpForce = 0.0f;

	UFUNCTION()
	void Launch(AActor* TargetActor);

	UFUNCTION()
	void Launch_Vec(AActor* TargetActor);
};
