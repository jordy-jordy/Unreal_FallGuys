// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/JumpShowDown/FootHold.h"

// Sets default values
AFootHold::AFootHold()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void AFootHold::BeginPlay()
{
	Super::BeginPlay();
	
	SetMesh();
}

// Called every frame
void AFootHold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFootHold::OperateMesh()
{
	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(RootScene);
	Platform->SetRelativeLocation({ 0, 0, 0 });
}

void AFootHold::SetMesh()
{
	Platform->SetStaticMesh(UFallGlobal::GetResourceMesh("FootHold"));
	Platform->SetMaterial(0, UFallGlobal::GetResourceMeshMaterial("FootHold"));
}

