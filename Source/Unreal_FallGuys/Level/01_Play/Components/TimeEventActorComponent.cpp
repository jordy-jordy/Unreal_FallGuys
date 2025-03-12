// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Components/TimeEventActorComponent.h"
#include "TimeEventActorComponent.h"

// Sets default values for this component's properties
UTimeEventActorComponent::UTimeEventActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTimeEventActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTimeEventActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (true == Events.IsEmpty())
	{
		return;
	}

	for (size_t i = 0; i < Events.Num(); i++)
	{
		FTimeEvent& Event = Events[i];
		Event.CurTime += DeltaTime;
		if (nullptr != Event.UpdateFunction)
		{
			Event.UpdateFunction(DeltaTime, Event.CurTime);
		}
	}

	for (size_t i = 0; i < Events.Num(); i++)
	{
		FTimeEvent& Event = Events[i];
		if (Event.MaxTime < Event.CurTime)
		{
			if (nullptr != Event.EndFunction)
			{
				Event.EndFunction();
			}

			if (false == Event.IsLoop)
			{
				Events.RemoveAt(i);
				--i;
			}
			else
			{
				Event.CurTime = 0.0f;
			}
		}
	}
}

void UTimeEventActorComponent::AddEvent(float _Time, TFunction<void(float, float)> _UpdateFunction, TFunction<void()> _EndFunction, bool _IsLoop)
{
	FTimeEvent& NewEvent = Events.Emplace_GetRef();
	NewEvent.IsLoop = _IsLoop;
	NewEvent.MaxTime = _Time;
	NewEvent.CurTime = 0.0f;
	NewEvent.EndFunction = _EndFunction;
	NewEvent.UpdateFunction = _UpdateFunction;
}

void UTimeEventActorComponent::AddUpdateEvent(float _Time, TFunction<void(float, float)> _UpdateFunction, bool _IsLoop)
{
	AddEvent(_Time, _UpdateFunction, nullptr, _IsLoop);
}

void UTimeEventActorComponent::AddEndEvent(float _Time, TFunction<void()> _EndFunction, bool _IsLoop)
{
	AddEvent(_Time, nullptr, _EndFunction, _IsLoop);
}

