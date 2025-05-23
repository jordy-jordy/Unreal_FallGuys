// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Race/PunchBox.h"

// Sets default values
APunchBox::APunchBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void APunchBox::BeginPlay()
{
	Super::BeginPlay();
	
	SetMesh();
}

// Called every frame
void APunchBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APunchBox::OperateMesh()
{
	// ActorComponent
	ObstacleMoveComp = CreateDefaultSubobject<UObsMovementActorComponent>(FName("ObstacleMoveComp"));

	// Mesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	RootScene->SetRelativeRotation({ 0, 0, 0 });

	PunchBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PunchBox"));
	PunchBox->SetupAttachment(RootScene);
	PunchBox->SetRelativeLocation({0, 0, 0});

	Punch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Punch"));
	Punch->SetupAttachment(PunchBox);
	Punch->SetRelativeLocation({0, 340, -10});

	PunchCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PunchCollision"));
	PunchCollision->SetupAttachment(Punch);
	PunchCollision->SetRelativeLocation({10, 10, 0});
	PunchCollision->SetRelativeRotation(FRotator{ 0, 0, 90 });
	PunchCollision->SetCapsuleSize(126.0f, 169.275f);

	Spring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spring"));
	Spring->SetupAttachment(Punch);
	Spring->SetRelativeLocation({ 0, -120, 10 });

	PillarBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarM"));
	PillarBody->SetupAttachment(PunchBox);
	PillarBody->SetRelativeLocation({0, -45, -764});
	PillarBody->SetRelativeScale3D({ 0.43f, 0.43f, 1.675f });

	PillarTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarT"));
	PillarTop->SetupAttachment(PillarBody);
	PillarTop->SetRelativeLocation({0.0f, 0.0f, 342.31f});
	PillarTop->SetRelativeRotation(FRotator{ 0, 0, 180 });
	PillarTop->SetRelativeScale3D({ 1.071f, 1.071f, 0.367f});

	PillarBottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarB"));
	PillarBottom->SetupAttachment(PillarBody);
	PillarBottom->SetRelativeLocation({0.0f, 0.0f, 22.013f});
	PillarBottom->SetRelativeScale3D({ 1.071f, 1.071f , 0.367f });

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(PunchBox);
	Platform->SetRelativeLocation({0.0f, -45.0f, -943.3f});
	Platform->SetRelativeScale3D({ 0.43f, 0.43f, 1.0f });
}

void APunchBox::SetMesh()
{
	PunchBox->SetStaticMesh(UFallGlobal::GetResourceMesh("PunchBox_Box"));

	Punch->SetStaticMesh(UFallGlobal::GetResourceMesh("PunchBox_Punch"));

	Spring->SetStaticMesh(UFallGlobal::GetResourceMesh("PunchBox_Spring"));

	PillarBody->SetStaticMesh(UFallGlobal::GetResourceMesh("PunchBox_PillarM"));

	PillarTop->SetStaticMesh(UFallGlobal::GetResourceMesh("PunchBox_PillarTB"));

	PillarBottom->SetStaticMesh(UFallGlobal::GetResourceMesh("PunchBox_PillarTB"));
	
	Platform->SetStaticMesh(UFallGlobal::GetResourceMesh("PunchBox_Platform"));
}
