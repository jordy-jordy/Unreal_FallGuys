// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/JumpShowDown/ShowDownStage.h"

#include "Kismet/GameplayStatics.h" 
#include "Unreal_FallGuys.h"
#include "Mode/01_Play/PlayGameMode.h"

// Sets default values
AShowDownStage::AShowDownStage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShowDownStage::BeginPlay()
{
	Super::BeginPlay();
	
	SetPlatforms();
}

// Called every frame
void AShowDownStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShowDownStage::SetPlatforms()
{
	// 서버에서만 스폰
	if (nullptr == GetWorld()->GetAuthGameMode())
	{
		return;
	}

	UWorld* World = GetWorld();

	if (World && BP_Platforms_Blue && BP_Platforms_Orange)
	{
		for (int32 i = 0; i < 8; i++)
		{
			FVector SpawnLocation = FVector(-10.0f, -40.0f, 0.0f);
			FRotator SpawnRotation = FRotator(0, -135 + (i * 45), 0);
			AShowDownPlatform* NewPlatform = nullptr;

			if (i % 2 == 0)
			{
				NewPlatform = World->SpawnActor<AShowDownPlatform>(BP_Platforms_Blue, SpawnLocation, SpawnRotation);
			}
			else
			{
				NewPlatform = World->SpawnActor<AShowDownPlatform>(BP_Platforms_Orange, SpawnLocation, SpawnRotation);
			}

			if (NewPlatform)
			{
				Platforms.Add(NewPlatform);
			}
		}
	}

	// 순서 섞기
	for (int i = 0; i < 100; i++)
	{
		int Left = FMath::RandRange(0, Platforms.Num() - 1);
		int Right = FMath::RandRange(0, Platforms.Num() - 1);

		AShowDownPlatform* Temp = Platforms[Left];
		Platforms[Left] = Platforms[Right];
		Platforms[Right] = Temp;
	}
}

void AShowDownStage::DownPlatforms(float DeltaTime)
{
	if (nullptr == GetWorld()->GetAuthGameMode())
	{
		return;
	}

	if (true == Platforms.IsEmpty())
	{
		return;
	}
	                             
	CurDownTime -= DeltaTime;

	if (0.0f >= CurDownTime)
	{
		Platforms[0]->IsLive = false;
		Platforms.RemoveAt(0); 
		CurDownTime = DownTime;
		return; 
	}
}
