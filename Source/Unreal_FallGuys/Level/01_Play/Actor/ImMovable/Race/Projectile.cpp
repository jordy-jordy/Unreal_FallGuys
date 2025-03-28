// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/Race/Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	LaunchComponent->LaunchForce = 800.0f;
	LaunchComponent->JumpForce = 240.0f;
	
	if (Projectile)
	{
		Projectile->OnComponentHit.AddDynamic(this, &AProjectile::OnComponentHit);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->GetInstigatorController())
		{
			FVector ZeroForce = FVector(0.f, 0.f, 0.f);
			HitComponent->SetPhysicsLinearVelocity(ZeroForce);
			HitComponent->SetPhysicsAngularVelocityInDegrees(ZeroForce);

			LaunchComponent->Launch(OtherActor);
		}
	}
}

void AProjectile::OperateMesh()
{
	// ActorComponent
	LaunchComponent = CreateDefaultSubobject<ULaunchActorComponent>(TEXT("Launch"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	RootScene->SetRelativeRotation({ 0, -90, 0 });

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	Projectile->SetupAttachment(RootScene);
	Projectile->SetRelativeLocation({ 0, 0, 0 });
	Projectile->SetRelativeScale3D({ 2.0f, 2.0f, 2.0f });

	SetMesh();
}

void AProjectile::SetMesh()
{
	UStaticMesh* BallMesh = LoadObject<UStaticMesh>(nullptr, *Ball);
	if (BallMesh)
	{
		Projectile->SetStaticMesh(BallMesh);
	}
}