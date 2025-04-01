// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/JumpShowDown/ShowDownStick.h"


// Sets default values
AShowDownStick::AShowDownStick()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void AShowDownStick::BeginPlay()
{
	Super::BeginPlay();

	SetMesh();
}

// Called every frame
void AShowDownStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShowDownStick::OperateMesh()
{
	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	BigStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BigStick"));
	BigStick->SetupAttachment(RootScene);
	BigStick->SetRelativeLocation({ 0, 0, 0 });
	BigStick->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	BigStick->SetRelativeScale3D({ 4.5f, 3.5f, 3.0f });

	BigStickCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BigStickCollision"));
	BigStickCollision->SetupAttachment(BigStick);
	BigStickCollision->SetBoxExtent(FVector(18.5f, 521.0f, 18.5f));
	BigStickCollision->SetRelativeLocation({ 0.0f, 0.0f, 62.0f });
	
	SmallStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SmallStick"));
	SmallStick->SetupAttachment(RootScene);
	SmallStick->SetRelativeLocation({ 0, 0, -45 });
	SmallStick->SetRelativeScale3D({ 2.0f, 3.25f, 1.0f });

	SmallStickCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SmallStickCollision"));
	SmallStickCollision->SetupAttachment(SmallStick);
	SmallStickCollision->SetBoxExtent(FVector(16.28f, 519.38f, 16.154f));
	SmallStickCollision->SetRelativeLocation({ 0.0f, 0.0f, 59.0f });
}

void AShowDownStick::SetMesh()
{
	BigStick->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateStick_Straight"));
	SmallStick->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateStick_Straight"));
}