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

void UMovementActorComponent::Move_Scene(float DeltaTime, USceneComponent* Target)
{
	Target->AddRelativeLocation(MoveSpeed * DeltaTime);
}

void UMovementActorComponent::Move(float DeltaTime, UStaticMeshComponent* Target)
{
	Target->AddRelativeLocation(MoveSpeed * DeltaTime);
}

void UMovementActorComponent::MoveCycle(float DeltaTime, UStaticMeshComponent* Target, EMoveAxis Axis)
{
	float LeftValue = 0.0f;
	float RightValue = 0.0f;

	switch (Axis)
	{
	case EMoveAxis::ROLL:
		LeftValue = Target->GetRelativeLocation().X;
		RightValue = LimitPos.X;
		break;
	case EMoveAxis::PITCH:
		LeftValue = Target->GetRelativeLocation().Y;
		RightValue = LimitPos.Y;
		break;
	case EMoveAxis::YAW:
		LeftValue = Target->GetRelativeLocation().Z;
		RightValue = LimitPos.Z;
		break;
	}

	if (IsMoveCycleLeft)
	{
		if (LeftValue >= -RightValue)
		{
			Target->AddRelativeLocation(MoveCycleSpeed * -DeltaTime);
		}
		else
		{
			IsMoveCycleLeft = false;
		}
	}
	else
	{
		if (LeftValue <= RightValue)
		{
			Target->AddRelativeLocation(MoveCycleSpeed * DeltaTime);
		}
		else
		{
			IsMoveCycleLeft = true;
		}
	}
}

void UMovementActorComponent::Spin(float DeltaTime, UStaticMeshComponent* Target)
{
	Target->AddRelativeRotation(SpinSpeed * DeltaTime);
}

void UMovementActorComponent::SpinOnce(float DeltaTime, UStaticMeshComponent* Target, EMoveAxis Axis)
{
	float LeftValue = 0.0f;
	float RightValue = 0.0f;

	switch (Axis)
	{
	case EMoveAxis::ROLL:
		LeftValue = Target->GetRelativeRotation().Roll;
		RightValue = StopAngle.Roll;
		break;
	case EMoveAxis::PITCH:
		LeftValue = Target->GetRelativeRotation().Pitch;
		RightValue = StopAngle.Pitch;
		break;
	case EMoveAxis::YAW:
		LeftValue = Target->GetRelativeRotation().Yaw;
		RightValue = StopAngle.Yaw;
		break;
	}

	if (RightValue > 0 && LeftValue - RightValue >= 0)
	{
		Target->AddRelativeRotation(SpinOnceSpeed * DeltaTime);
	}
	else if (RightValue < 0 && LeftValue - RightValue <= 0)
	{
		Target->AddRelativeRotation(SpinOnceSpeed * DeltaTime);
	}
}

void UMovementActorComponent::SpinCycle(float DeltaTime, UStaticMeshComponent* Target, EMoveAxis Axis)
{
	float LeftValue = 0.0f;
	float RightValue = 0.0f;

	switch (Axis)
	{
	case EMoveAxis::ROLL:
		LeftValue = Target->GetRelativeRotation().Roll;
		RightValue = LimitAngle.Roll;
		break;
	case EMoveAxis::PITCH:
		LeftValue = Target->GetRelativeRotation().Pitch;
		RightValue = LimitAngle.Pitch;
		break;
	case EMoveAxis::YAW:
		LeftValue = Target->GetRelativeRotation().Yaw;
		RightValue = LimitAngle.Yaw;
		break;
	}

	if (IsSpinCycleLeft)
	{
		if (LeftValue >= -RightValue)
		{
			Target->AddRelativeRotation(SpinCycleSpeed * -DeltaTime);
		}
		else
		{
			IsSpinCycleLeft = false;
		}
	}
	else
	{
		if (LeftValue <= RightValue)
		{
			Target->AddRelativeRotation(SpinCycleSpeed * DeltaTime);
		}
		else
		{
			IsSpinCycleLeft = true;
		}
	}
}