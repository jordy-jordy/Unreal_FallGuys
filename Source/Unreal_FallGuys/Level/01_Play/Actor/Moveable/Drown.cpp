// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Drown.h"

// Sets default values
ADrown::ADrown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetDrownLocation();
}

// Called when the game starts or when spawned
void ADrown::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->IsSpinLeft = IsLeft;
	MovementComponent->SpinSpeed = FRotator({ 0.0f, 180.0f, 0.0f });
	
	SetDrownMesh();
	SetDrownRotation();
}

// Called every frame
void ADrown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpinPropeller(DeltaTime);
}

void ADrown::SetDrownLocation()
{
	// ActorComponent
	MovementComponent = CreateDefaultSubobject<UMovementActorComponent>(FName("MovementComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	DrownBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownBody"));
	DrownBody->SetupAttachment(RootScene);
	DrownBody->SetRelativeLocation({ 0, 0, 0 });

	DrownPropeller_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownPropeller_Left"));
	DrownPropeller_Left->SetupAttachment(DrownBody);
	DrownPropeller_Left->SetRelativeLocation({ -135, -1, 100 });

	DrownPropeller_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownPropeller_Right"));
	DrownPropeller_Right->SetupAttachment(DrownBody);
	DrownPropeller_Right->SetRelativeLocation({ 135, -1, 100 });

	DrownLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownLight"));
	DrownLight->SetupAttachment(RootScene);
	DrownLight->SetRelativeLocation({ 0, 0, 0 });

	DrownLightArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownLightArrow"));
	DrownLightArrow->SetupAttachment(DrownLight);
	DrownLightArrow->SetRelativeLocation({ 0, 0, 0 });
}

void ADrown::SetDrownRotation()
{
	// Rotation
	float BodyAngle = 0.0f;
	float LightAngle = 0.0f;
	float ArrowAngle = 0.0f;

	if (IsLeft)
	{
		BodyAngle = 45.0f;
		LightAngle = 45.0f;
		ArrowAngle = 40.0f;
	}
	else
	{
		BodyAngle = 135.0f;
		LightAngle = -45.0f;
		ArrowAngle = -225.0f;
	}

	RootScene->SetRelativeRotation({ 0, BodyAngle , 0 });
	DrownLight->SetRelativeRotation({ 0, LightAngle, 0 });
	DrownLightArrow->SetRelativeRotation({ 0, ArrowAngle, 0 });
}

void ADrown::SetDrownMesh_B()
{
	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *DrownBody_B);
	if (BodyMesh)
	{
		DrownBody->SetStaticMesh(BodyMesh);
	}

	UStaticMesh* LeftPropellerMesh = LoadObject<UStaticMesh>(nullptr, *LeftPropeller_B);
	if (LeftPropellerMesh)
	{
		DrownPropeller_Left->SetStaticMesh(LeftPropellerMesh);
	}
	
	UStaticMesh* RightPropellerMesh = LoadObject<UStaticMesh>(nullptr, *RightPropeller_B);
	if (RightPropellerMesh)
	{
		DrownPropeller_Right->SetStaticMesh(RightPropellerMesh);
	}

	UStaticMesh* LightMesh = LoadObject<UStaticMesh>(nullptr, *DrownLight_B);
	if (LightMesh)
	{
		DrownLight->SetStaticMesh(LightMesh);
	}

	UStaticMesh* ArrowMesh = LoadObject<UStaticMesh>(nullptr, *LightArrow_B);
	if (ArrowMesh)
	{
		DrownLightArrow->SetStaticMesh(ArrowMesh);
	}
}

void ADrown::SetDrownMesh_R()
{
	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *DrownBody_R);
	if (BodyMesh)
	{
		DrownBody->SetStaticMesh(BodyMesh);
	}

	UStaticMesh* LeftPropellerMesh = LoadObject<UStaticMesh>(nullptr, *LeftPropeller_R);
	if (LeftPropellerMesh)
	{
		DrownPropeller_Left->SetStaticMesh(LeftPropellerMesh);
	}

	UStaticMesh* RightPropellerMesh = LoadObject<UStaticMesh>(nullptr, *RightPropeller_R);
	if (RightPropellerMesh)
	{
		DrownPropeller_Right->SetStaticMesh(RightPropellerMesh);
	}

	UStaticMesh* LightMesh = LoadObject<UStaticMesh>(nullptr, *DrownLight_R);
	if (LightMesh)
	{
		DrownLight->SetStaticMesh(LightMesh);
	}

	UStaticMesh* ArrowMesh = LoadObject<UStaticMesh>(nullptr, *LightArrow_R);
	if (ArrowMesh)
	{
		DrownLightArrow->SetStaticMesh(ArrowMesh);
	}
}

void ADrown::SetDrownMesh_M()
{
	UStaticMesh* BodyMesh = LoadObject<UStaticMesh>(nullptr, *DrownBody_M);
	if (BodyMesh)
	{
		DrownBody->SetStaticMesh(BodyMesh);
	}

	UStaticMesh* LeftPropellerMesh = LoadObject<UStaticMesh>(nullptr, *LeftPropeller_M);
	if (LeftPropellerMesh)
	{
		DrownPropeller_Left->SetStaticMesh(LeftPropellerMesh);
	}

	UStaticMesh* RightPropellerMesh = LoadObject<UStaticMesh>(nullptr, *RightPropeller_M);
	if (RightPropellerMesh)
	{
		DrownPropeller_Right->SetStaticMesh(RightPropellerMesh);
	}

	UStaticMesh* LightMesh = LoadObject<UStaticMesh>(nullptr, *DrownLight_M);
	if (LightMesh)
	{
		DrownLight->SetStaticMesh(LightMesh);
	}

	UStaticMesh* ArrowMesh = LoadObject<UStaticMesh>(nullptr, *LightArrow_M);
	if (ArrowMesh)
	{
		DrownLightArrow->SetStaticMesh(ArrowMesh);
	}
}

void ADrown::SetDrownMesh()
{
	switch (DrownType)
	{
	case EDrownType::BLUE:
		SetDrownMesh_B();
		break;
	case EDrownType::RED:
		SetDrownMesh_R();
		break;
	case EDrownType::MINT:
		SetDrownMesh_M();
		break;
	}
}

void ADrown::SpinPropeller(float DeltaTime)
{
	float Delta_L = DeltaTime;
	float Delta_R = DeltaTime;

	(IsLeft) ? Delta_R *= -1 : Delta_L *= -1;
	
	MovementComponent->Spin(Delta_L, DrownPropeller_Left);
	MovementComponent->Spin(Delta_R, DrownPropeller_Right);
}

