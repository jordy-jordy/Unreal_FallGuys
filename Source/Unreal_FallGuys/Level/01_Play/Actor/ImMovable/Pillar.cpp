// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/Pillar.h"

// Sets default values
APillar::APillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ActorComponent
	LaunchComponent = CreateDefaultSubobject<ULaunchActorComponent>(FName("LaunchComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	PillarBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarBody"));
	PillarBody->SetupAttachment(RootScene);
	PillarBody->SetRelativeLocation({ 0, 0, 0 });
	PillarBody->SetCollisionProfileName(TEXT("CollisionProfile_LevelOBJ"));
	PillarBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void APillar::BeginPlay()
{
	Super::BeginPlay();
	
	SetDrownMesh();

	if (PillarBody)
	{
		PillarBody->OnComponentHit.AddDynamic(this, &APillar::OnComponentHit);
	}
}

// Called every frame
void APillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APillar::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (LaunchComponent && OtherActor && OtherActor != this)
	{
		LaunchComponent->Launch(OtherActor, LaunchForce);
	}
}

void APillar::SetDrownMesh_P()
{
	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *PinkPillar);
	if (BodyMesh)
	{
		PillarBody->SetStaticMesh(BodyMesh);
	}
}

void APillar::SetDrownMesh_S()
{
	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *ScarletPillar);
	if (BodyMesh)
	{
		PillarBody->SetStaticMesh(BodyMesh);
	}
}

void APillar::SetDrownMesh()
{
	switch (PillarType)
	{
	case EPillarType::PINK:
		SetDrownMesh_P();
		break;
	case EPillarType::SCARLET:
		SetDrownMesh_S();
		break;
	}
}

