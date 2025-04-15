// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RhinoSpawnManager.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API ARhinoSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARhinoSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Rhino|Factory")
	TSubclassOf<class AActor> RhinoFactory;

	UPROPERTY(BlueprintReadWrite, Category = "Rhino|Actor")
	TArray<class AActor*> Rhinos;

};
