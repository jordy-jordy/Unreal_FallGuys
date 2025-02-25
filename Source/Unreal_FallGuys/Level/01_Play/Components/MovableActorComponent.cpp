// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Components/MovableActorComponent.h"

// Sets default values for this component's properties
UMovableActorComponent::UMovableActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentYaw = -50.0f; // �ʱ� ����
	RotationSpeed = 1.0f; // ȸ�� �ӵ�
	MaxYaw = 50.0f; // �ִ� ����
	MinYaw = -50.0f; // �ּ� ����
}


// Called when the game starts
void UMovableActorComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentYaw = GetOwner()->GetActorRotation().Pitch;
	
}


// Called every frame
void UMovableActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentYaw += RotationSpeed * DeltaTime * 50.0f; // DeltaTime�� ���Ͽ� ������ ������ ȸ��

	// ������ �ִ� �Ǵ� �ּҿ� �����ϸ� ���� ��ȯ
	if (CurrentYaw >= MaxYaw || CurrentYaw <= MinYaw)
	{
		RotationSpeed = -RotationSpeed; // ���� ��ȯ
	}

	// ȸ�� ����
	FRotator NewRotation = FRotator(CurrentYaw, 0.0f, 0.0f); 
	GetOwner()->SetActorRotation(NewRotation);

}

