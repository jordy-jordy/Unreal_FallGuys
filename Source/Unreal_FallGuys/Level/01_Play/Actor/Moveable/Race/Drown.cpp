// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Race/Drown.h"

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
	ObstacleMoveComp = CreateDefaultSubobject<UObsMovementActorComponent>(FName("ObstacleMoveComp"));

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
	DrownLight->SetupAttachment(DrownBody);
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
	DrownBody->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Blue_Body"));

	DrownPropeller_Left->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Blue_Propeller_L"));
	
	DrownPropeller_Right->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Blue_Propeller_R"));

	DrownLight->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Blue_Light"));

	DrownLightArrow->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Blue_Arrow"));
}

void ADrown::SetDrownMesh_R()
{
	DrownBody->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Red_Body"));

	DrownPropeller_Left->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Red_Propeller_L"));

	DrownPropeller_Right->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Red_Propeller_R"));

	DrownLight->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Red_Light"));

	DrownLightArrow->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Red_Arrow"));
}

void ADrown::SetDrownMesh_M()
{
	DrownBody->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Mint_Body"));

	DrownPropeller_Left->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Mint_Propeller_L"));

	DrownPropeller_Right->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Mint_Propeller_R"));

	DrownLight->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Mint_Light"));

	DrownLightArrow->SetStaticMesh(UFallGlobal::GetResourceMesh("Drawn_Mint_Arrow"));
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
}

