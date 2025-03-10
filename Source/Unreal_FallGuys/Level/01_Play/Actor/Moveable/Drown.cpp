// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Drown.h"

// Sets default values
ADrown::ADrown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	DrownBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownBody"));
	DrownBody->SetupAttachment(RootScene);
	DrownBody->SetRelativeLocation({ 0, 0, 0 });

	DrownPropeller_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownPropeller_Left"));
	DrownPropeller_Left->SetupAttachment(RootScene);
	DrownPropeller_Left->SetRelativeLocation({ 135, -1, 100 });

	DrownPropeller_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownPropeller_Right"));
	DrownPropeller_Right->SetupAttachment(RootScene);
	DrownPropeller_Right->SetRelativeLocation({ -135, -1, 100 });

	DrownLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownLight"));
	DrownLight->SetupAttachment(DrownBody);
	DrownLight->SetRelativeLocation({ 0, 0, 0 });

	DrownLightArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrownLightArrow"));
	DrownLightArrow->SetupAttachment(DrownLight);
	DrownLightArrow->SetRelativeLocation({ 0, 0, 0 });
}

// Called when the game starts or when spawned
void ADrown::BeginPlay()
{
	Super::BeginPlay();
	
	SetDrownDirection();
}

// Called every frame
void ADrown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrown::SetDrownDirection()
{
	float BodyAngle = 0.0f;
	float LightAngle = 0.0f;
	float ArrowAngle = 0.0f;
	
	if (IsLeft)
	{
		BodyAngle = 45.0f;
		LightAngle = 45.0f;
		ArrowAngle = 40.0f;
	}
	else
	{
		BodyAngle = 135.0f;
		LightAngle = -45.0f;
		ArrowAngle = 225.0f;
	}

	RootScene->SetRelativeRotation({ 0, BodyAngle , 0 });
	DrownLight->SetRelativeRotation({0, LightAngle, 0});
	DrownLightArrow->SetRelativeRotation({0, ArrowAngle, 0});
}

