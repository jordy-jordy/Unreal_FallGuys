// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/BoxSpace.h"
#include "Components/ChildActorComponent.h"
#include "Level/01_Play/Actor/PushBox.h"

// Sets default values
ABoxSpace::ABoxSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SceneComp->SetupAttachment(RootComponent);

	//for (int i = 1; i < 5; i++)
	//{
	//	 UChildActorComponent* ChildActorComp= CreateDefaultSubobject<UChildActorComponent>(TEXT(""));
	//	 ChildActorComp->SetupAttachment(SceneComp);

	//}


}

// Called when the game starts or when spawned
void ABoxSpace::BeginPlay()
{
	Super::BeginPlay();
	//if (PushBoxFactory != nullptr)
	//{
	//	//PushBox = Cast<APushBox>(GetWorld()->SpawnActor<APushBox>(PushBoxFactory, GetActorTransform()));
	//	/*if (nullptr != PushBox)
	//	{*/
	//	ChildActorComp->SetChildActorClass(PushBoxFactory);
	//	ChildActorComp->CreateChildActor();
	//	ChildActorComp->CreateChildActor();
	//	ChildActorComp->CreateChildActor();
	//	ChildActorComp->CreateChildActor();
	//	ChildActorComp->CreateChildActor();
	//	ChildActorComp->CreateChildActor();
	//	//PushBox->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	////}
	//}
	

}

// Called every frame
void ABoxSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

