// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/02_End/EndPawn.h"

// Sets default values
AEndPawn::AEndPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEndPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEndPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

