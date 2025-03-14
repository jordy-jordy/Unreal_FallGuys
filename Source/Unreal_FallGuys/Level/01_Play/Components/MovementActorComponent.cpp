// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Components/MovementActorComponent.h"

// Sets default values for this component's properties
UMovementActorComponent::UMovementActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMovementActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMovementActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMovementActorComponent::Spin(float DeltaTime, USceneComponent* RootScene)
{
	if (IsSpinLeft)
	{
		RootScene->AddRelativeRotation(SpinSpeed * DeltaTime);
	}
	else
	{
		RootScene->AddRelativeRotation(SpinSpeed * -DeltaTime);
	}
}

