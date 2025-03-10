// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/RandomMap.h"
#include <Kismet/GameplayStatics.h>
#include "Global/FallGlobal.h"

// Sets default values
ARandomMap::ARandomMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARandomMap::BeginPlay()
{
	Super::BeginPlay();

	

	for (FString MapName : UFallGlobal::GetAvailableLevels())
	{
		MapList.Add(MapName);

	}

	
	
	
}

// Called every frame
void ARandomMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ARandomMap::GetRandomLevel()
{
	//if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == GetOw GetController())
	{



		int Random = 0;
		int RandomIndex = 0;
		FString MapName = "";
		do {
			Random = FMath::RandRange(1, MapList.Num());
			RandomIndex = Random - 1;

		} while (PlayedMapList.Contains(RandomIndex)); // 이미 선택된 맵인지 확인

		PlayedMapList.Add(RandomIndex); // 선택된 맵 추가
		MapName = MapList[RandomIndex];

		return MapList[RandomIndex]; // 랜덤으로 선택된 맵 반환



	}
}

