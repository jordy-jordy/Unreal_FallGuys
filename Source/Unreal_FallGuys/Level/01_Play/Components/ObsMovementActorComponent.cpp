// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Components/ObsMovementActorComponent.h"

// Sets default values for this component's properties
UObsMovementActorComponent::UObsMovementActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UObsMovementActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UObsMovementActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UObsMovementActorComponent::CompareVector(FVector First, FVector Second, EObsAxis AxisType)
{
	switch (AxisType)
	{
	case EObsAxis::X:
		return (First.X - Second.X > 0);
	case EObsAxis::Y:
		return (First.Y - Second.Y > 0);
	case EObsAxis::Z:
		return (First.Z - Second.Z > 0);
	default:
		return false;
	}
}

bool UObsMovementActorComponent::CompareRotator(FRotator First, FRotator Second, EObsAxis AxisType)
{
	switch (AxisType)
	{
	case EObsAxis::X:
		return (First.Roll - Second.Roll > 0);
	case EObsAxis::Y:
		return (First.Pitch - Second.Pitch > 0);
	case EObsAxis::Z:
		return (First.Yaw - Second.Yaw > 0);
	default:
		return false;
	}
}

void UObsMovementActorComponent::Move_Scene(USceneComponent* Target, FVector MoveSpeed, float DeltaTime)
{
	Target->SetRelativeLocation(Target->GetRelativeLocation() + MoveSpeed * DeltaTime);
}

void UObsMovementActorComponent::Move_Static(UStaticMeshComponent* Target, FVector MoveSpeed, float DeltaTime)
{
	Target->SetRelativeLocation(Target->GetRelativeLocation() + MoveSpeed * DeltaTime);
}

void UObsMovementActorComponent::MoveOnce(UStaticMeshComponent* Target, FVector MoveSpeed, FVector LimitPos, float DeltaTime)
{
	if (LimitPos.X > 0 || LimitPos.Y > 0 || LimitPos.Z > 0)
	{
		Target->SetRelativeLocation(Target->GetRelativeLocation() + MoveSpeed * DeltaTime);
	}
	else if (LimitPos.X < 0 || LimitPos.Y < 0 || LimitPos.Z < 0)
	{
		Target->SetRelativeLocation(Target->GetRelativeLocation() + MoveSpeed * DeltaTime);
	}
}

void UObsMovementActorComponent::MoveCycle(UStaticMeshComponent* Target, FVector MoveSpeed, FVector LimitPos, EObsAxis Axis, float DeltaTime)
{
	if (IsLeft)
	{
		if (CompareVector(Target->GetRelativeLocation(), -LimitPos, Axis))
		{
			Target->SetRelativeLocation(Target->GetRelativeLocation() + MoveSpeed * -DeltaTime);
		}
		else
		{
			IsLeft = !IsLeft;
		}
	}
	else
	{
		if (CompareVector(Target->GetRelativeLocation(), LimitPos, Axis))
		{
			IsLeft = !IsLeft;
		}
		else
		{
			Target->SetRelativeLocation(Target->GetRelativeLocation() + MoveSpeed * DeltaTime);
		}
	}
}

void UObsMovementActorComponent::Spin(UStaticMeshComponent* Target, FRotator RotateSpeed, float DeltaTime)
{
	Target->SetRelativeRotation(Target->GetRelativeRotation() + RotateSpeed * DeltaTime);
}

void UObsMovementActorComponent::SpinOnce(UStaticMeshComponent* Target, FRotator RotateSpeed, FRotator LimitAngle, float DeltaTime)
{
	if (LimitAngle.Roll > 0 || LimitAngle.Pitch > 0 || LimitAngle.Yaw > 0)
	{
		Target->SetRelativeRotation(Target->GetRelativeRotation() + RotateSpeed * DeltaTime);		
	}
	else if (LimitAngle.Roll < 0 || LimitAngle.Pitch < 0 || LimitAngle.Yaw < 0)
	{
			Target->SetRelativeRotation(Target->GetRelativeRotation() + RotateSpeed * DeltaTime);
	}
}

void UObsMovementActorComponent::SpinCycle(UStaticMeshComponent* Target, FRotator RotateSpeed, FRotator LimitAngle, EObsAxis Axis, float DeltaTime)
{
	if (false == GetOwner()->HasAuthority())
	{
		return;
	}

	if (IsLeft)
	{

	}
	else
	{

	}
}

