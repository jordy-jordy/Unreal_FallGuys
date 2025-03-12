// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/PushBox.h"

// Sets default values
APushBox::APushBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APushBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APushBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

