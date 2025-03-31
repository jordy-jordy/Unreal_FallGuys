// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/JumpShowDown/ShowDownStick.h"

// Sets default values
AShowDownStick::AShowDownStick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void AShowDownStick::BeginPlay()
{
	Super::BeginPlay();
	
	SetMesh();
}

// Called every frame
void AShowDownStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccSticks(DeltaTime);
}

void AShowDownStick::OperateMesh()
{

}

void AShowDownStick::SetMesh()
{
}

void AShowDownStick::AccSticks(float DeltaTime)
{
	// 하단 : 35 / 상단 : 32
	// 속도 증가율 0.8f

	MovementComponent->AccSpin(DeltaTime, BigStick, FRotator(0.0f, 32.0f, 0.0f), FRotator(0.0f, 0.8f, 0.0f));
	MovementComponent->AccSpin(DeltaTime, SmallStick, FRotator(0.0f, 35.0f, 0.0f), FRotator(0.0f, 0.8f, 0.0f));
}

