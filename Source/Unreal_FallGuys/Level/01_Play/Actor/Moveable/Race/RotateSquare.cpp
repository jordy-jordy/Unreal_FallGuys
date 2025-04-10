// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Race/RotateSquare.h"

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

	SetMesh();
}

// Called every frame
void ARotateSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotateSquare::OperateMesh()
{
	// ActorComponent
	ObstacleMoveComp = CreateDefaultSubobject<UObsMovementActorComponent>(FName("ObstacleMoveComp"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	RotateAxis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateAxis"));
	RotateAxis->SetupAttachment(RootScene);
	RotateAxis->SetRelativeLocation({ 0, 0, 0 });

	RotateSquare = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateSquare"));
	RotateSquare->SetupAttachment(RotateAxis);
	RotateSquare->SetRelativeLocation({ 0, 0, 460 });
}

void ARotateSquare::SetMesh()
{
	RotateAxis->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateSquare_Axis"));

	RotateSquare->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateSquare_Body"));
}

