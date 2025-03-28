// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/JumpShowDown/ShowDownPlatform.h"

// Sets default values
AShowDownPlatform::AShowDownPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void AShowDownPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	(IsBlue) ? SetMesh_Blue() : SetMesh_Orange();
}

// Called every frame
void AShowDownPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShowDownPlatform::OperateMesh()
{
	// ActorComponent
	MovementComponent = CreateDefaultSubobject<UMovementActorComponent>(FName("MovementComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(RootScene);
	Platform->SetRelativeLocation({ 0, 0, 0 });
	Platform->SetRelativeRotation(FRotator (180, 0, -90));
	Platform->SetRelativeScale3D({ 0.8f, 0.8f, 0.8f });
}

void AShowDownPlatform::SetMesh_Orange()
{
	Platform->SetStaticMesh(UFallGlobal::GetResourceMesh("ShowDownPlatform_Orange"));
	Platform->SetMaterial(0, UFallGlobal::GetResourceMeshMaterial("ShowDownPlatform_Orange"));
}

void AShowDownPlatform::SetMesh_Blue()
{
	Platform->SetStaticMesh(UFallGlobal::GetResourceMesh("ShowDownPlatform_Blue"));
	Platform->SetMaterial(0, UFallGlobal::GetResourceMeshMaterial("ShowDownPlatform_Blue"));
}

