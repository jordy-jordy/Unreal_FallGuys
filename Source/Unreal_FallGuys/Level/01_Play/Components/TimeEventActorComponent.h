// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeEventActorComponent.generated.h"


USTRUCT()
struct UNREAL_FALLGUYS_API FTimeEvent
{
	GENERATED_BODY()

public:
	bool IsLoop = false;
	// std::function
	TFunction<void(float, float)> UpdateFunction;
	TFunction<void()> EndFunction;
	float CurTime = 0.0f;
	float MaxTime = 0.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FALLGUYS_API UTimeEventActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeEventActorComponent();

	void AddEvent(float _Time, TFunction<void(float, float)> _UpdateFunction, TFunction<void()> _EndFunction, bool _IsLoop = false);

	void AddEndEvent(float _Time, TFunction<void()> _EndFunction, bool _IsLoop = false);

	void AddUpdateEvent(float _Time, TFunction<void(float, float)> _UpdateFunction, bool _IsLoop = false);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<FTimeEvent> Events;
};
