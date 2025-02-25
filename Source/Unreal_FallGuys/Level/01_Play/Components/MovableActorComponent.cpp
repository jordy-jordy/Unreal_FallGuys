// Fill out your copyright notice in the Description page of Project Settings.

#include "Level/01_Play/Components/MovableActorComponent.h"

// Sets default values for this component's properties
UMovableActorComponent::UMovableActorComponent()
{
	CurrentYaw = -50.0f; // 초기 각도
	RotationSpeed = 1.0f; // 회전 속도
	MaxYaw = 50.0f; // 최대 각도
	MinYaw = -50.0f; // 최소 각도
	// ...
}

// Called when the game starts
void UMovableActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UMovableActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 현재 각도 업데이트
	CurrentYaw += RotationSpeed;

	// 각도가 최대 또는 최소에 도달하면 방향 전환
	if (CurrentYaw >= MaxYaw || CurrentYaw <= MinYaw)
	{
		RotationSpeed = -RotationSpeed; // 방향 전환
	}

	// 회전 적용
	FRotator NewRotation = FRotator(0.0f, CurrentYaw, 0.0f); // Yaw 축으로 변경
	GetOwner()->SetActorRotation(NewRotation);
}