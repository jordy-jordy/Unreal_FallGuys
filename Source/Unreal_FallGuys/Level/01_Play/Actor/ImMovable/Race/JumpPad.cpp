// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/Race/JumpPad.h"

// Sets default values
AJumpPad::AJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OparateMesh();
}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
	Super::BeginPlay();

	SetMesh();

	if (Pad)
	{
		Pad->OnComponentHit.AddDynamic(this, &AJumpPad::OnComponentHit);
	}
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpPad::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (LaunchComponent && OtherActor && OtherActor != this)
	{
		LaunchComponent->Launch_Vec(OtherActor);
	}
}

void AJumpPad::OparateMesh()
{
	// ActorComponent
	LaunchComponent = CreateDefaultSubobject<ULaunchActorComponent>(FName("LaunchComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Case = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JumpPadCase"));
	Case->SetupAttachment(RootScene);
	Case->SetRelativeLocation({ 0, 0, 0 });

	Pad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JumpPad"));
	Pad->SetupAttachment(Case);
	Pad->SetRelativeLocation({ 0, 0, 67 });

	Light = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JumpPadLight"));
	Light->SetupAttachment(Case);
	Light->SetRelativeLocation({ 0, 0, 0 });

	LaunchComponent->JumpForce = 1500.0f;
}

void AJumpPad::SetMesh()
{
	Case->SetStaticMesh(UFallGlobal::GetResourceMesh("JumpPad_Case"));

	Pad->SetStaticMesh(UFallGlobal::GetResourceMesh("JumpPad_Pad"));

	Light->SetStaticMesh(UFallGlobal::GetResourceMesh("JumpPad_Light"));
}

