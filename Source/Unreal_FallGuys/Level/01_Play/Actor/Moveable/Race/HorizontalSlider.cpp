// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Race/HorizontalSlider.h"

// Sets default values
AHorizontalSlider::AHorizontalSlider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OparateMesh();
}

// Called when the game starts or when spawned
void AHorizontalSlider::BeginPlay()
{
	Super::BeginPlay();

	SetMesh();
	SetComponent();
}

// Called every frame
void AHorizontalSlider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementComponent->MoveCycle(DeltaTime, Slider, EMoveAxis::ROLL);
}

void AHorizontalSlider::OparateMesh()
{
	// ActorComponent
	MovementComponent = CreateDefaultSubobject<UMovementActorComponent>(FName("MovementComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;


	SliderLine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SliderLine"));
	SliderLine->SetupAttachment(RootScene);
	SliderLine->SetRelativeRotation({ 0, 90, 0 });

	Slider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Slider"));
	Slider->SetupAttachment(SliderLine);
	Slider->SetRelativeLocation({ 0, 0, 70 });
}


void AHorizontalSlider::SetMesh()
{
	SliderLine->SetStaticMesh(UFallGlobal::GetResourceMesh("Slider_Line"));

	Slider->SetStaticMesh(UFallGlobal::GetResourceMesh("Slider_Body"));
}

void AHorizontalSlider::SetComponent()
{
	// SetMesh
	if (MovementComponent->IsMoveCycleLeft)
	{
		Slider->SetRelativeLocation({ 280, 0, 70 });
	}
	else
	{
		Slider->SetRelativeLocation({ -280, 0, 70 });
	}

	// SetComponent
	MovementComponent->MoveCycleSpeed = FVector({ 320, 0, 0 });
	MovementComponent->LimitPos = FVector({ 280, 0, 0 });
}
