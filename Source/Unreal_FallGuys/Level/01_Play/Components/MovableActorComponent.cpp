// Fill out your copyright notice in the Description page of Project Settings.

#include "Level/01_Play/Components/MovableActorComponent.h"

// Sets default values for this component's properties
UMovableActorComponent::UMovableActorComponent()
{
	CurrentYaw = -50.0f; // �ʱ� ����
	RotationSpeed = 1.0f; // ȸ�� �ӵ�
	MaxYaw = 50.0f; // �ִ� ����
	MinYaw = -50.0f; // �ּ� ����
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
	// ���� ���� ������Ʈ
	CurrentYaw += RotationSpeed;

	// ������ �ִ� �Ǵ� �ּҿ� �����ϸ� ���� ��ȯ
	if (CurrentYaw >= MaxYaw || CurrentYaw <= MinYaw)
	{
		RotationSpeed = -RotationSpeed; // ���� ��ȯ
	}

	// ȸ�� ����
	FRotator NewRotation = FRotator(0.0f, CurrentYaw, 0.0f); // Yaw ������ ����
	GetOwner()->SetActorRotation(NewRotation);
}