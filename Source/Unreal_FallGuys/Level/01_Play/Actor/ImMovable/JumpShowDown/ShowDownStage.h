// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShowDownPlatform.h"
#include "ShowDownStage.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AShowDownStage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShowDownStage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platforms_Blue")
	TSubclassOf<AShowDownPlatform> BP_Platforms_Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platforms_Orange")
	TSubclassOf<AShowDownPlatform> BP_Platforms_Orange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platforms")
	float DownTime = 10.0f;

	float CurDownTime = DownTime;

private:
	TArray<AShowDownPlatform*> Platforms;

	UFUNCTION()
	void SetPlatforms();

	UFUNCTION(BlueprintCallable)
	void DownPlatforms(float DeltaTime);
};
