// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/Race/MovingPad.h"

// Sets default values
AMovingPad::AMovingPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void AMovingPad::BeginPlay()
{
	Super::BeginPlay();

	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMovingPad::OnBeginOverlap);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AMovingPad::OnEndOverlap);
	}
}

// Called every frame
void AMovingPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMove)
	{
		MovementComponent->Move_Scene(DeltaTime, Player->GetRootComponent());
	}
}

void AMovingPad::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		Player = OtherActor;
		IsMove = true;
	}
}

void AMovingPad::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		IsMove = false;
	}
}

void AMovingPad::OperateMesh()
{
	// ActorComponent
	MovementComponent = CreateDefaultSubobject<UMovementActorComponent>(FName("MovementComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	RootScene->SetRelativeRotation({ 0, -90, 0 });

	Pad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pad"));
	Pad->SetupAttachment(RootScene);
	Pad->SetRelativeLocation({ 0, 0, 0 });

	Cover = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cover"));
	Cover->SetupAttachment(RootScene);
	Cover->SetRelativeLocation({ 0, 0, 23 });
	Cover->SetRelativeScale3D({ 3.8f, 7.7f, 0.015f });
	Cover->SetVisibility(false);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Pad);
	BoxCollision->SetRelativeLocation({ 0, 0, 20 });
	BoxCollision->SetBoxExtent(FVector{192.3f, 396.6f, 10.0f});
	
	SetMesh();
}

void AMovingPad::SetMesh()
{
	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *PadBody);
	if (BodyMesh)
	{
		Pad->SetStaticMesh(BodyMesh);
	}

	UStaticMesh* CoverMesh = LoadObject<UStaticMesh>(nullptr, *PadCover);
	if (CoverMesh)
	{
		Cover->SetStaticMesh(CoverMesh);
	}
}
