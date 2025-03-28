// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/Race/Bumper.h"

// Sets default values
ABumper::ABumper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OparateMesh();
}

// Called when the game starts or when spawned
void ABumper::BeginPlay()
{
	Super::BeginPlay();
	
	SetBumperMesh();

	if (BumperBody)
	{
		BumperBody->OnComponentHit.AddDynamic(this, &ABumper::OnComponentHit);
	}
}

// Called every frame
void ABumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABumper::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (LaunchComponent && OtherActor && OtherActor != this)
	{
		LaunchComponent->Launch(OtherActor);
	}
}

void ABumper::OparateMesh()
{
	// ActorComponent
	LaunchComponent = CreateDefaultSubobject<ULaunchActorComponent>(FName("LaunchComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	BumperBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BumperBody"));
	BumperBody->SetupAttachment(RootScene);
	BumperBody->SetRelativeLocation({ 0, 0, 0 });
}

void ABumper::SetBumperMesh()
{
	switch (BumperType)
	{
	case EBumperType::PINKPILLAR:
		BumperBody->SetStaticMesh(UFallGlobal::GetResourceMesh("Bumper_Pink"));
		break;
	case EBumperType::SCARLETPILLAR:
		BumperBody->SetStaticMesh(UFallGlobal::GetResourceMesh("Bumper_Scarlar"));
		break;
	case EBumperType::TRIBUMPER:
		BumperBody->SetStaticMesh(UFallGlobal::GetResourceMesh("Bumper_Tri"));
		break;
	case EBumperType::SQBUMPER:
		BumperBody->SetStaticMesh(UFallGlobal::GetResourceMesh("Bumper_Sq"));
		break;
	}
}

