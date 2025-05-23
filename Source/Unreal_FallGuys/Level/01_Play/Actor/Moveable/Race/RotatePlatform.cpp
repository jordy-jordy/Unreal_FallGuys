// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Race/RotatePlatform.h"

// Sets default values
ARotatePlatform::ARotatePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OparateMesh();
}

// Called when the game starts or when spawned
void ARotatePlatform::BeginPlay()
{
	Super::BeginPlay();

	if (StickType != EStickType::NONE && StickBody)
	{
		StraightCollision->OnComponentBeginOverlap.AddDynamic(this, &ARotatePlatform::OnBeginOverlap);
		CrossCollision_X->OnComponentBeginOverlap.AddDynamic(this, &ARotatePlatform::OnBeginOverlap);
		CrossCollision_Y->OnComponentBeginOverlap.AddDynamic(this, &ARotatePlatform::OnBeginOverlap);
		LaunchComponent->SetActive(true);
	}

	SetRotateMesh();
}

// Called every frame
void ARotatePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotatePlatform::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (LaunchComponent && OtherActor && OtherActor != this)
	{
		LaunchComponent->Launch(OtherActor);
	}
}

void ARotatePlatform::OparateMesh()
{
	// ActorComponent
	ObstacleMoveComp = CreateDefaultSubobject<UObsMovementActorComponent>(FName("ObstacleMoveComp"));
	LaunchComponent = CreateDefaultSubobject<ULaunchActorComponent>(FName("LaunchComponent"));
	LaunchComponent->SetActive(false);

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	PlatformAxis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateAxis"));
	PlatformAxis->SetupAttachment(RootScene);
	PlatformAxis->SetRelativeLocation({ 0, 0, 0 });

	PlatformBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatePlatform"));
	PlatformBody->SetupAttachment(PlatformAxis);
	PlatformBody->SetRelativeLocation({ 0, 0, 0 });

	StickBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateStick"));
	StickBody->SetupAttachment(RootScene);
	StickBody->SetRelativeLocation({ 0, 0, 0 });

	StraightCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("StraightCollision"));
	StraightCollision->SetupAttachment(StickBody);
	StraightCollision->SetCapsuleSize(22.0f, 525.5f);
	StraightCollision->SetRelativeLocation({3, 0, 60});
	StraightCollision->SetRelativeRotation(FRotator(0, 0, -90));
	StraightCollision->SetActive(false);

	CrossCollision_X = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CrossCollision1"));
	CrossCollision_X->SetupAttachment(StickBody);
	CrossCollision_X->SetCapsuleSize(43.45f, 705.0f);
	CrossCollision_X->SetRelativeLocation({ 0, -2, 167 });
	CrossCollision_X->SetRelativeRotation(FRotator(0, 90, -90));
	CrossCollision_X->SetActive(false);

	CrossCollision_Y = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CrossCollision2"));
	CrossCollision_Y->SetupAttachment(StickBody);
	CrossCollision_Y->SetCapsuleSize(43.45f, 705.0f);
	CrossCollision_Y->SetRelativeLocation({ 0, -2, 167 });
	CrossCollision_Y->SetRelativeRotation(FRotator(0, 0, -90));
	CrossCollision_Y->SetActive(false);
}

#pragma region MyRegion
void ARotatePlatform::SetPlatform_Y()
{
	PlatformAxis->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePlatform_Yellow_Axis"));

	PlatformBody->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePlatform_Yellow_Body"));
}

void ARotatePlatform::SetPlatform_P()
{
	PlatformAxis->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePlatform_Purple_Axis"));

	PlatformBody->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePlatform_Purple_Body"));
}

void ARotatePlatform::SetPlatform_R()
{
	PlatformAxis->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePlatform_Red_Axis"));

	PlatformBody->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePlatform_Red_Body"));
}

void ARotatePlatform::SetPlatform_F()
{
	PlatformAxis->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePlatform_Fan_Axis"));

	PlatformBody->SetStaticMesh(UFallGlobal::GetResourceMesh("RotatePlatform_Fan_Body"));
}

void ARotatePlatform::SetPlatform()
{
	switch (PlatformType)
	{
	case EPlatformType::NONE:
		PlatformAxis->SetStaticMesh(nullptr);
		PlatformBody->SetStaticMesh(nullptr);
		break;
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
	StickBody->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateStick_Straight"));

	StraightCollision->SetActive(true);

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
	StickBody->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateStick_Cross"));

	CrossCollision_X->SetActive(true);
	CrossCollision_Y->SetActive(true);

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
