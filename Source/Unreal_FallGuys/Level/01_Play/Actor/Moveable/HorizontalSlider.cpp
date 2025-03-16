// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/HorizontalSlider.h"

// Sets default values
AHorizontalSlider::AHorizontalSlider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHorizontalSlider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHorizontalSlider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

