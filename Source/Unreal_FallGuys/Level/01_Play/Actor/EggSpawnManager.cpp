// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/EggSpawnManager.h"
#include "Level/01_Play/Actor/Egg.h"

// Sets default values
AEggSpawnManager::AEggSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEggSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < EggCount; i++)
	{
		Eggs.Add(GetWorld()->SpawnActor<AEgg>(EggFactory, GetActorTransform()));
	}
	
}

// Called every frame
void AEggSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

