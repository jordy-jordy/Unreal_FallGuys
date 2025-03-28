// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Egg.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API AEgg : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEgg();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Team", meta = (AllowPrivateAccess = "true"))
	int Team = 0;

};
