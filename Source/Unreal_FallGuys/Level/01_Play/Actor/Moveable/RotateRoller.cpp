// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/RotateRoller.h"

// Sets default values
ARotateRoller::ARotateRoller()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void ARotateRoller::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComponent->SpinCycleSpeed = FRotator({ 0, 0, 70 });
	MovementComponent->LimitAngle = FRotator({ 0, 0, 70 });
}

// Called every frame
void ARotateRoller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementComponent->SpinCycle(DeltaTime, RollerBody, EMoveAxis::ROLL);
	MovementComponent->SpinCycle(DeltaTime, RollerAxis, EMoveAxis::ROLL);
}

void ARotateRoller::OperateMesh()
{
	// ActorComponent
	MovementComponent = CreateDefaultSubobject<UMovementActorComponent>(FName("MovementComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	RollerAxis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RollerAxis"));
	RollerAxis->SetupAttachment(RootScene);
	RollerAxis->SetRelativeLocation({ 0, 0, 0 });

	RollerBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RollerBody"));
	RollerBody->SetupAttachment(RollerAxis);
	RollerBody->SetRelativeLocation({ 0, 0, -385 });

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RollerCollision"));
	CapsuleCollision->SetupAttachment(RollerBody);
	CapsuleCollision->SetCapsuleSize(75.0f, 316.4f);
	CapsuleCollision->SetRelativeRotation(FRotator(90, 0, 0));

	SetMesh();
}

void ARotateRoller::SetMesh()
{
	UStaticMesh* AxisMesh = LoadObject<UStaticMesh>(nullptr, *Axis);
	if (AxisMesh)
	{
		RollerAxis->SetStaticMesh(AxisMesh);
	}

	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *Body);
	if (BodyMesh)
	{
		RollerBody->SetStaticMesh(BodyMesh);
	}
}