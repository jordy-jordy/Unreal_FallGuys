// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/JumpShowDown/ShowDownStage.h"

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
	
	/*if (nullptr != GetWorld()->GetAuthGameMode())
	{
		GetWorld()->GetAuthGameMode()->AddShowDown(this);
	}*/

	// TArray<AShowDownStage>
}

// Called every frame
void AShowDownStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShowDownStage::SetPlatforms()
{
	UWorld* World = GetWorld();

	if (World)
	{
		for (int32 i = 0; i < 8; i++)
		{
			FVector SpawnLocation = FVector(-10.0f, -40.0f, 0.0f);
			FRotator SpawnRotation = FRotator(0, -135 + (i * 45), 0);
			AShowDownPlatform* NewPlatform = World->SpawnActor<AShowDownPlatform>(AShowDownPlatform::StaticClass(), SpawnLocation, SpawnRotation);

			if (NewPlatform)
			{
				Platforms.Add(NewPlatform);
			}

			Platforms[i]->IsBlue = (i % 2 == 0);
			(Platforms[i]->IsBlue) ? NewPlatform->SetMesh_Blue() : NewPlatform->SetMesh_Orange();
		}
	}
}

void AShowDownStage::SetFallPlatform()
{
	int32 Num = FMath::RandRange(0, 7);

	if (true == Platforms[Num]->IsLive)
	{
		UE_LOG(LogTemp, Warning, TEXT("FallPlatform Number is : %d"), Num);
		Platforms[Num]->SetActorLocation(GetActorLocation() + FVector(0, 0, -10));
		Platforms[Num]->IsLive = false;
	}
	else
	{
		SetFallPlatform();
	}
}

