// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitleyPawn.h"

// Sets default values
ATitleyPawn::ATitleyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATitleyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATitleyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATitleyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

