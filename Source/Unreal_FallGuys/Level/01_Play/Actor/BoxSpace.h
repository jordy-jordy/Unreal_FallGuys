// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxSpace.generated.h"

UCLASS()
class UNREAL_FALLGUYS_API ABoxSpace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxSpace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Factory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APushBox> PushBoxFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "ChildActor", meta = (AllowPrivateAccess = "true"))
	TArray<class APushBox*> PushBoxs;

	UPROPERTY(EditAnywhere, Category = "ChildActor", meta = (AllowPrivateAccess = "true"))
	class APushBox* PushBox;


	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "ChildActor", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildActorComp;
};
