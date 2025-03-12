// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Components/MoveCycleActorComponent.h"
#include "MoveCycleActorComponent.h"

// Sets default values for this component's properties
UMoveCycleActorComponent::UMoveCycleActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveCycleActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMoveCycleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveCycle(DeltaTime);
}

void UMoveCycleActorComponent::Move(float DeltaTime)
{
	// Tick 
	// Limit를 초과하는지 체크하고 IsLeft 변동
	// IsLeft에 따라서 움직임 실행
}

void UMoveCycleActorComponent::Spin(float DeltaTime)
{
	if (LimitMovingVector == FVector::ZeroVector)
	{
		if (IsSpinLeft)
		{
			
		}
		else
		{

		}
	}
}

void UMoveCycleActorComponent::SetDirection()
{

}

void UMoveCycleActorComponent::MoveCycle(float DeltaTime)
{
	if (AddLocation != FVector::ZeroVector)
	{
		Move(DeltaTime);
	}

	if (AddRotation != FRotator::ZeroRotator)
	{
		Spin(DeltaTime);
	}
}

