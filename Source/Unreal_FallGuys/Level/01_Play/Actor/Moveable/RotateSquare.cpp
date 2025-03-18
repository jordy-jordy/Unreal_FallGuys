// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/RotateSquare.h"

// Sets default values
ARotateSquare::ARotateSquare()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void ARotateSquare::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARotateSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementComponent->Spin(DeltaTime, RotateSquare);
}

void ARotateSquare::OperateMesh()
{
	// ActorComponent
	MovementComponent = CreateDefaultSubobject<UMovementActorComponent>(FName("MovementComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	RotateAxis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateAxis"));
	RotateAxis->SetupAttachment(RootScene);
	RotateAxis->SetRelativeLocation({ 0, 0, 0 });
	RotateAxis->SetCollisionProfileName(TEXT("CollisionProfile_LevelOBJ"));
	RotateAxis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	RotateSquare = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateSquare"));
	RotateSquare->SetupAttachment(RotateAxis);
	RotateSquare->SetRelativeLocation({ 0, 0, 460 });
	RotateSquare->SetCollisionProfileName(TEXT("CollisionProfile_LevelOBJ"));
	RotateSquare->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SetMesh();
}

void ARotateSquare::SetMesh()
{
	UStaticMesh* AxisMesh = LoadObject<UStaticMesh>(nullptr, *Axis);
	if (AxisMesh)
	{
		RotateAxis->SetStaticMesh(AxisMesh);
	}

	UStaticMesh* SqureMesh = LoadObject<UStaticMesh>(nullptr, *Square);
	if (SqureMesh)
	{
		RotateSquare->SetStaticMesh(SqureMesh);
	}
}

