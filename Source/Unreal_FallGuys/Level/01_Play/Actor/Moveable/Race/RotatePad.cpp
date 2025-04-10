// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Race/RotatePad.h"

// Sets default values
ARotatePad::ARotatePad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void ARotatePad::BeginPlay()
{
	Super::BeginPlay();

	SetMesh();
}

// Called every frame
void ARotatePad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotatePad::OperateMesh()
{
	// ActorComponent
	//ObstacleMoveComp = CreateDefaultSubobject<UObsMovementActorComponent>(FName("ObstacleMoveComp"));

	// Mesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	RootScene->SetRelativeRotation({ 0, 90, 0 });

	Axis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateAxis"));
	Axis->SetupAttachment(RootScene);
	Axis->SetRelativeLocation({ 0, 0, 0 });

	Pad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatePad"));
	Pad->SetupAttachment(Axis);
	Pad->SetRelativeLocation({ 0, 0, 0 });

	Cover = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatePadCover"));
	Cover->SetupAttachment(Pad);
	Cover->SetRelativeLocation({0, 208, 7});
	Cover->SetRelativeScale3D({3.8f, 3.92f, 0.1f});
	Cover->SetVisibility(false);
}

void ARotatePad::SetMesh()
{
	Axis->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePad_Axis"));

	Pad->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePad_Pad"));

	Cover->SetStaticMesh(UFallGlobal::GetResourceMesh("Cube"));
}