// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Components/MovableActorComponent.h"

// Sets default values for this component's properties
UMovableActorComponent::UMovableActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicated(true);
	//CurrentPivot = -50.0f; // �ʱ� ����
	RotationSpeed = 1.0f; // ȸ�� �ӵ�
	MaxPivot = 50.0f; // �ִ� ����
	MinPivot = -50.0f; // �ּ� ����
}


// Called when the game starts
void UMovableActorComponent::BeginPlay()
{
	Super::BeginPlay();

	RotationSpeed = 1.0f; // ȸ�� �ӵ�

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

	// ������ �ִ� �Ǵ� �ּҿ� �����ϸ� ���� ��ȯ
	if (CurrentPivot >= MaxPivot || CurrentPivot <= MinPivot)
	{
		RotationSpeed = -RotationSpeed; // ���� ��ȯ
	}
	CurrentPivot += RotationSpeed * DeltaTime * 50.0f; // DeltaTime�� ���Ͽ� ������ ������ ȸ��

	// ȸ�� ����
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

