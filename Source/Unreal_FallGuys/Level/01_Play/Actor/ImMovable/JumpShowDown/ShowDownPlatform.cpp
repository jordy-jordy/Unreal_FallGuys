// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/JumpShowDown/ShowDownPlatform.h"

// Sets default values
AShowDownPlatform::AShowDownPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void AShowDownPlatform::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShowDownPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr == GetWorld()->GetAuthGameMode())
	{
		return;
	}

	if (true == IsLive)
	{
		OriginPos = GetActorLocation();
		return;
	}

	if (0.0f <= ShakeTime)
	{
		FVector Vec = FMath::VRand();
		SetActorLocation(OriginPos + (Vec * 10.0f));
		ShakeTime -= DeltaTime;
		return;
	}
	
	AddActorLocalOffset(FVector::DownVector * 800.0f * DeltaTime);

}

void AShowDownPlatform::OperateMesh()
{
	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(RootScene);
	Platform->SetRelativeLocation({ 0, 0, 0 });
	Platform->SetRelativeRotation(FRotator (180, 0, -90));
	Platform->SetRelativeScale3D({ 0.8f, 0.8f, 0.8f });
}
