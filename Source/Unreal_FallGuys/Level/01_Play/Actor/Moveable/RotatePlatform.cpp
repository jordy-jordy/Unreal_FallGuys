// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/RotatePlatform.h"

// Sets default values
ARotatePlatform::ARotatePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetMesh();
}

// Called when the game starts or when spawned
void ARotatePlatform::BeginPlay()
{
	Super::BeginPlay();
	
	SetRotateMesh();

	if (StickType != EStickType::NONE && StickBody)
	{
		StickBody->OnComponentHit.AddDynamic(this, &ARotatePlatform::OnComponentHit);
		LaunchComponent->SetActive(true);
	}
}

// Called every frame
void ARotatePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementComponent->Spin(DeltaTime, RootScene);
}

void ARotatePlatform::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (LaunchComponent && OtherActor && OtherActor != this)
	{
		LaunchComponent->Launch(OtherActor);
	}
}

void ARotatePlatform::SetMesh()
{
	if (PlatformType == EPlatformType::NONE && StickType == EStickType::NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("EmptyType is Not Rendering!"));
	}

	// ActorComponent
	MovementComponent = CreateDefaultSubobject<UMovementActorComponent>(FName("MovementComponent"));
	LaunchComponent = CreateDefaultSubobject<ULaunchActorComponent>(FName("LaunchComponent"));
	LaunchComponent->SetActive(false);


	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	PlatformAxis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateAxis"));
	PlatformAxis->SetupAttachment(RootScene);
	PlatformAxis->SetRelativeLocation({ 0, 0, 0 });
	PlatformAxis->SetCollisionProfileName(TEXT("CollisionProfile_LevelOBJ"));
	PlatformAxis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	PlatformBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatePlatform"));
	PlatformBody->SetupAttachment(RootScene);
	PlatformBody->SetRelativeLocation({ 0, 0, 0 });
	PlatformBody->SetCollisionProfileName(TEXT("CollisionProfile_LevelOBJ"));
	PlatformBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	StickBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateStick"));
	StickBody->SetupAttachment(RootScene);
	StickBody->SetRelativeLocation({ 0, 0, 0 });
	StickBody->SetCollisionProfileName(TEXT("CollisionProfile_LevelOBJ"));
	StickBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

#pragma region MyRegion
void ARotatePlatform::SetPlatform_Y()
{
	UStaticMesh* AxisMesh = LoadObject<UStaticMesh>(nullptr, *PlatformAxis_Y);
	if (AxisMesh)
	{
		PlatformAxis->SetStaticMesh(AxisMesh);
	}

	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *PlatformBody_Y);
	if (BodyMesh)
	{
		PlatformBody->SetStaticMesh(BodyMesh);
	}
}

void ARotatePlatform::SetPlatform_P()
{
	UStaticMesh* AxisMesh = LoadObject<UStaticMesh>(nullptr, *PlatformAxis_P);
	if (AxisMesh)
	{
		PlatformAxis->SetStaticMesh(AxisMesh);
	}

	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *PlatformBody_P);
	if (BodyMesh)
	{
		PlatformBody->SetStaticMesh(BodyMesh);
	}
}

void ARotatePlatform::SetPlatform_R()
{
	UStaticMesh* AxisMesh = LoadObject<UStaticMesh>(nullptr, *PlatformAxis_R);
	if (AxisMesh)
	{
		PlatformAxis->SetStaticMesh(AxisMesh);
	}

	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *PlatformBody_R);
	if (BodyMesh)
	{
		PlatformBody->SetStaticMesh(BodyMesh);
	}
}

void ARotatePlatform::SetPlatform_F()
{
	UStaticMesh* AxisMesh = LoadObject<UStaticMesh>(nullptr, *PlatformAxis_F);
	if (AxisMesh)
	{
		PlatformAxis->SetStaticMesh(AxisMesh);
	}

	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *PlatformBody_F);
	if (BodyMesh)
	{
		PlatformBody->SetStaticMesh(BodyMesh);
	}
}

void ARotatePlatform::SetPlatform()
{
	switch (PlatformType)
	{
	case EPlatformType::YELLOW:
		SetPlatform_Y();
		break;
	case EPlatformType::PURPLE:
		SetPlatform_P();
		break;
	case EPlatformType::RED:
		SetPlatform_R();
		break;
	case EPlatformType::FAN:
		SetPlatform_F();
		break;
	}
}
#pragma endregion

#pragma region SetStick
void ARotatePlatform::SetStick_S()
{
	UStaticMesh* StickMesh = LoadObject<UStaticMesh>(nullptr, *StraightStick);
	if (StickMesh)
	{
		StickBody->SetStaticMesh(StickMesh);
	}

	// Set Location And Scale
	switch (PlatformType)
	{
	case EPlatformType::YELLOW:
		break;
	case EPlatformType::PURPLE:
	case EPlatformType::RED:
		StickBody->SetRelativeLocation({ 0, 0, 65 });
		break;
	case EPlatformType::FAN:
		StickBody->SetRelativeLocation({ 0, 0, 36 });
		break;
	}
}

void ARotatePlatform::SetStick_C()
{
	UStaticMesh* StickMesh = LoadObject<UStaticMesh>(nullptr, *CrossStick);
	if (StickMesh)
	{
		StickBody->SetStaticMesh(StickMesh);
	}

	// Set Location And Scale
	switch (PlatformType)
	{
	case EPlatformType::YELLOW:
		StickBody->SetRelativeLocation({ 0, 0, 10 });
		StickBody->SetRelativeScale3D({ 0.48f, 0.48f, 0.48f });
		break;
	case EPlatformType::PURPLE:
	case EPlatformType::RED:
		break;
	case EPlatformType::FAN:
		break;
	}
}

void ARotatePlatform::SetStick()
{
	switch (StickType)
	{
	case EStickType::STRAIGHT:
		SetStick_S();
		break;
	case EStickType::CROSS:
		SetStick_C();
		break;
	}
}
#pragma endregion

void ARotatePlatform::SetRotateMesh()
{
	if (PlatformType != EPlatformType::NONE)
	{
		SetPlatform();
	}

	if (StickType != EStickType::NONE)
	{
		SetStick();
	}
}
