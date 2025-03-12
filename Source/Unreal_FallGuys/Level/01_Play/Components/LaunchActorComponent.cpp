// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Components/LaunchActorComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
ULaunchActorComponent::ULaunchActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULaunchActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void ULaunchActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULaunchActorComponent::Launch(AActor* TartgetActor)
{
	if (ACharacter* Character = Cast<ACharacter>(TartgetActor))
	{
		FVector SelfLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = TartgetActor->GetActorLocation();
		FVector LaunchDirection = (TargetLocation - SelfLocation).GetSafeNormal();
		FVector LaunchVelocity = LaunchDirection * LaunchForce;
		LaunchVelocity.Z = JumpForce;
		Character->LaunchCharacter(LaunchVelocity, false, false);
	}
}