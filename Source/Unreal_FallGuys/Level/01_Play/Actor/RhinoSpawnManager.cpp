// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/RhinoSpawnManager.h"

// Sets default values
ARhinoSpawnManager::ARhinoSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARhinoSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARhinoSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

