// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/RotatePad.h"

// Sets default values
ARotatePad::ARotatePad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void ARotatePad::BeginPlay()
{
	Super::BeginPlay();
	
	TimeEventComponent->AddUpdateEvent(1.0f, [this](float _Delta, float _Acc)
		{
			MoveUp(_Delta, _Acc);
		});
}

// Called every frame
void ARotatePad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotatePad::OperateMesh()
{
	// ActorComponent
	TimeEventComponent = CreateDefaultSubobject<UTimeEventActorComponent>(FName("TimeEvnetComponent"));

	// Mesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	RootScene->SetRelativeRotation({ 0, 90, 0 });

	Axis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateAxis"));
	Axis->SetupAttachment(RootScene);
	Axis->SetRelativeLocation({ 0, 0, 0 });

	Pad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatePad"));
	Pad->SetupAttachment(Axis);
	Pad->SetRelativeLocation({ 0, 0, 0 });

	Cover = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatePadCover"));
	Cover->SetupAttachment(Pad);
	Cover->SetRelativeLocation({0, 208, 7});
	Cover->SetRelativeScale3D({3.8f, 3.92f, 0.1f});
	Cover->SetVisibility(false);
	
	SetMesh();
}

void ARotatePad::SetMesh()
{
	UStaticMesh* AxisMesh = LoadObject<UStaticMesh>(nullptr, *PadAxis);
	if (AxisMesh)
	{
		Axis->SetStaticMesh(AxisMesh);
	}

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

void ARotatePad::MoveUp(float _Delta, float _Acc)
{
	float Vel = FMath::Lerp(0, LimitAngle, _Acc);

	if (1.0f <= _Acc)
	{
		_Acc = 1.0f;
	}

	Axis->SetRelativeRotation(FQuat::MakeFromEuler({ Vel, 0.0f, 0.0f }));

	if (1.0f <= _Acc)
	{
		TimeEventComponent->AddUpdateEvent(1.0f, [this](float _Delta, float _Acc)
			{
				MoveDown(_Delta, _Acc);
			});
	}
}

void ARotatePad::MoveDown(float _Delta, float _Acc)
{
	float Vel = FMath::Lerp(LimitAngle, 0, _Acc);

	if (1.0f <= _Acc)
	{
		_Acc = 1.0f;
	}

	Axis->SetRelativeRotation(FQuat::MakeFromEuler({ Vel, 0.0f, 0.0f }));

	if (1.0f <= _Acc)
	{
		TimeEventComponent->AddEndEvent(1.0f, [this]()
			{
				TimeEventComponent->AddUpdateEvent(1.0f, [this](float _Delta, float _Acc)
					{
						MoveUp(_Delta, _Acc);
					});
			});
	}
}
