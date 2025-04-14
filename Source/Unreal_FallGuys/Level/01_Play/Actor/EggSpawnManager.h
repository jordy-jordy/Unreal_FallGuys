// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EggSpawnManager.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AEggSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEggSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	TArray<class AEgg*> GetCurEggs() const
	{
		return Eggs;
	}

	UFUNCTION()
	TArray<int> CheckEggTeam();

	UPROPERTY(EditAnywhere, Category = "Egg|Factory")
	TSubclassOf<class AActor> EggFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg")
	TArray<class AEgg*> Eggs;

	UPROPERTY(EditAnywhere, Category = "Egg")
	int EggCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg")
	TArray<int> TeamEggCount;

	UFUNCTION(BlueprintCallable)
	TArray<int> GetTeamEggCount() const

	{
		int a = TeamEggCount.Num();
		if (TeamEggCount.IsEmpty())
		{ 

			
			return TArray<int>(); // 비어 있을 경우 빈 배열 반환
		}

		return TeamEggCount;

	}
	
	
};
