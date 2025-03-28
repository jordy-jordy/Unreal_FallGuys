// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Egg.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEgg::AEgg()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEgg::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEgg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEgg::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEgg, Team);
	
}
