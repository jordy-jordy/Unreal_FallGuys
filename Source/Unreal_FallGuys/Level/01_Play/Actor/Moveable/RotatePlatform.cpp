// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/RotatePlatform.h"

// Sets default values
ARotatePlatform::ARotatePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARotatePlatform::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlatformType == EPlatformType::NONE && StickType == EStickType::NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("EmptyType is Not Rendering!"));
	}
}

// Called every frame
void ARotatePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARotatePlatform::SetRotateMesh()
{
}

