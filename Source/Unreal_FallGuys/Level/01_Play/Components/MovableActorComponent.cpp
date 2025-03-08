// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Components/MovableActorComponent.h"

// Sets default values for this component's properties
UMovableActorComponent::UMovableActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicated(true);
	//CurrentPivot = -50.0f; // 초기 각도
	RotationSpeed = 1.0f; // 회전 속도
	MaxPivot = 50.0f; // 최대 각도
	MinPivot = -50.0f; // 최소 각도
}


// Called when the game starts
void UMovableActorComponent::BeginPlay()
{
	Super::BeginPlay();

	RotationSpeed = 1.0f; // 회전 속도

	switch (AxisType)
	{
	case EAxisType::Roll:
		CurrentPivot = GetOwner()->GetActorRotation().Roll;

		break;
	case EAxisType::Pitch:
		CurrentPivot = GetOwner()->GetActorRotation().Pitch;

		break;
	case EAxisType::Yaw:
		CurrentPivot = GetOwner()->GetActorRotation().Yaw;

		break;
	default:
		break;
	}

}


// Called every frame
void UMovableActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 각도가 최대 또는 최소에 도달하면 방향 전환
	if (CurrentPivot >= MaxPivot || CurrentPivot <= MinPivot)
	{
		RotationSpeed = -RotationSpeed; // 방향 전환
	}
	CurrentPivot += RotationSpeed * DeltaTime * 50.0f; // DeltaTime을 곱하여 프레임 독립적 회전

	// 회전 적용
	FRotator NewRotation = FRotator(0.0f, 0.0f, 0.0f);

	switch (AxisType)
	{
	case EAxisType::Roll:
		NewRotation = FRotator(0.0f, 0.0f, CurrentPivot);
		break;
	case EAxisType::Pitch:
		NewRotation = FRotator(CurrentPivot, 0.0f, 0.0f);
		break;
	case EAxisType::Yaw:
		NewRotation = FRotator(0.0f, CurrentPivot, 0.0f);
		break;
	default:
		break;
	}
	GetOwner()->SetActorRotation(NewRotation);



}

