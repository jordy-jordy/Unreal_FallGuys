// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Race/RotateRoller.h"

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

	SetMesh();
}

// Called every frame
void ARotateRoller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotateRoller::OperateMesh()
{
	// ActorComponent
	ObstacleMoveComp = CreateDefaultSubobject<UObsMovementActorComponent>(FName("ObstacleMoveComp"));

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
}

void ARotateRoller::SetMesh()
{
	RollerAxis->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateRollor_Axis"));

	RollerBody->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateRollor_Body"));
}