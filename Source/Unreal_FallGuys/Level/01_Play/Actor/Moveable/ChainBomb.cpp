// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/ChainBomb.h"

// Sets default values
AChainBomb::AChainBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void AChainBomb::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComponent->SpinCycleSpeed = FRotator({ 120, 0, 0 });
	MovementComponent->LimitAngle = FRotator({ 70, 0, 0 });
}

// Called every frame
void AChainBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementComponent->SpinCycle(DeltaTime, StartChain, EMoveAxis::PITCH);
}

void AChainBomb::OperateMesh()
{
	// ActorComponent
	MovementComponent = CreateDefaultSubobject<UMovementActorComponent>(FName("MovementComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Axis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChainAxis"));
	Axis->SetupAttachment(RootScene);
	Axis->SetRelativeLocation({ 0, 0, 0 });
	Axis->SetRelativeRotation(FRotator(0, 90, 0));

	StartChain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StartChain"));
	StartChain->SetupAttachment(RootScene);
	StartChain->SetRelativeLocation({ 0, 0, -85 });

	Bomb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb"));
	Bomb->SetupAttachment(StartChain);
	Bomb->SetRelativeLocation({ 0, 0, 0 });

	BombCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BombCollision"));
	BombCollision->SetSphereRadius(75.8f);
	BombCollision->SetupAttachment(Bomb);
	BombCollision->SetRelativeLocation({ 0, 0, -68 });

	SetMesh();
}

void AChainBomb::SetMesh()
{
	UStaticMesh* AxisMesh = LoadObject<UStaticMesh>(nullptr, *ChainAxis);
	if (AxisMesh)
	{
		Axis->SetStaticMesh(AxisMesh);
	}

	UStaticMesh* ChainMesh = LoadObject<UStaticMesh>(nullptr, *Chain);
	if (ChainMesh)
	{
		StartChain->SetStaticMesh(ChainMesh);
	}

	UStaticMesh* BoamMesh = LoadObject<UStaticMesh>(nullptr, *ChainBomb);
	if (BoamMesh)
	{
		Bomb->SetStaticMesh(BoamMesh);
	}
}

