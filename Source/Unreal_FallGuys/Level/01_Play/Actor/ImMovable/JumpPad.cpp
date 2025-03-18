// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/JumpPad.h"

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
	Case->SetCollisionProfileName(TEXT("CollisionProfile_LevelOBJ"));
	Case->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Pad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JumpPad"));
	Pad->SetupAttachment(Case);
	Pad->SetRelativeLocation({ 0, 0, 67 });
	Pad->SetCollisionProfileName(TEXT("CollisionProfile_LevelOBJ"));
	Pad->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Light = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JumpPadLight"));
	Light->SetupAttachment(Case);
	Light->SetRelativeLocation({ 0, 0, 0 });
	Light->SetCollisionProfileName(TEXT("CollisionProfile_LevelOBJ"));
	Light->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	LaunchComponent->JumpForce = 1500.0f;

	SetMesh();
}

void AJumpPad::SetMesh()
{
	UStaticMesh* CaseMesh = LoadObject<UStaticMesh>(nullptr, *PadCase);
	if (CaseMesh)
	{
		Case->SetStaticMesh(CaseMesh);
	}

	UStaticMesh* PadMesh = LoadObject<UStaticMesh>(nullptr, *JumpPad);
	if (PadMesh)
	{
		Pad->SetStaticMesh(PadMesh);
	}

	UStaticMesh* LightMesh = LoadObject<UStaticMesh>(nullptr, *PadLight);
	if (LightMesh)
	{
		Light->SetStaticMesh(LightMesh);
	}
}

