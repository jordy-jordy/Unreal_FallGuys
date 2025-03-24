// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/Cannon.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannon::OperateMesh()
{
	// ActorComponents
	TimeEventComponent = CreateDefaultSubobject<UTimeEventActorComponent>(TEXT("TimeEvnet"));
	
	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	RootScene->SetRelativeRotation({ 0, 90, 0 });

	Cannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon"));
	Cannon->SetupAttachment(RootScene);
	Cannon->SetRelativeLocation({ 0, 0, 0 });

	CannonBodyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CanonBodyCollision"));
	CannonBodyCollision->SetupAttachment(Cannon);
	CannonBodyCollision->SetRelativeLocation({ 0, 0, 0 });
	CannonBodyCollision->SetSphereRadius(335.011f);

	CannonLeftEyeCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CannonLeftEyeCollision"));
	CannonLeftEyeCollision->SetupAttachment(CannonBodyCollision);
	CannonLeftEyeCollision->SetRelativeLocation({ -118.544f, -23.0f, 66.744f });
	CannonLeftEyeCollision->SetRelativeRotation(FRotator(71.0f, 0.0f, 0.0f));
	CannonLeftEyeCollision->SetCapsuleSize(150.387f, 258.972f);

	CannonRightEyeCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CannonRightEyeCollision"));
	CannonRightEyeCollision->SetupAttachment(CannonBodyCollision);
	CannonRightEyeCollision->SetRelativeLocation({ 126.208f, -22.0f, 43.512f});
	CannonRightEyeCollision->SetRelativeRotation(FRotator(61.0f, 180.0f, 0.0f));
	CannonRightEyeCollision->SetCapsuleSize(150.387f, 258.972f);

	CannonMuzzleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CannonMuzzleCollision"));
	CannonMuzzleCollision->SetupAttachment(Cannon);
	CannonMuzzleCollision->SetRelativeLocation({ 0, 301, 0 });
	CannonMuzzleCollision->SetRelativeRotation(FRotator( 0, 0, -90 ));
	CannonMuzzleCollision->SetCapsuleSize(232.462f, 285.0f);

	SetMesh();
}

void ACannon::SetMesh()
{
	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *CannonBody);
	if (BodyMesh)
	{
		Cannon->SetStaticMesh(BodyMesh);
	}
}

