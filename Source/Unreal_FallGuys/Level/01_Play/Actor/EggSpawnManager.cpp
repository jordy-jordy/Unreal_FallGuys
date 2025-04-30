// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/EggSpawnManager.h"
#include "Level/01_Play/Actor/Egg.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEggSpawnManager::AEggSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TeamEggCount = TArray<int>();
}

// Called when the game starts or when spawned
void AEggSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEgg::StaticClass(), Eggs);
	for (int i = 0; i < EggCount; i++)
	{
		Eggs[i]->SetOwner(this);
	}
	
}

// Called every frame
void AEggSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TeamEggCount.Empty();
	TeamEggCount = CheckEggTeam();
}

TArray<int> AEggSpawnManager::CheckEggTeam()
{

	TArray<int> TeamEggCoopy;
	TeamEggCoopy.SetNum(4);
	for (AActor* egg : Eggs)
	{
		AEgg* Egg = Cast<AEgg>(egg);
		if (Egg->Team == -1) continue;
		else TeamEggCoopy[Egg->Team] += 1;
		
	}
	return TeamEggCoopy;
	
}

