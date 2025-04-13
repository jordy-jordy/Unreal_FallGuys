// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/02_End/EndPawn.h"

#include <Global/BaseGameInstance.h>
#include <Global/Data/GlobalDataTable.h>
#include <Global/FallGlobal.h>


// Sets default values
AEndPawn::AEndPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootSceneComp;
	UpComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Up"));
	UpComp->SetupAttachment(RootComponent);
	LowComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Low"));
	LowComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEndPawn::BeginPlay()
{
	Super::BeginPlay();

	// 이현정 : GameInstance에 저장된 코스튬 정보를 가져옴
	CostumeColor = UFallGlobal::GetCostumeColor(this);
	CostumeTop = UFallGlobal::GetCostumeTop(this);
	CostumeBot = UFallGlobal::GetCostumeBot(this);

	// 이현정 : GameInstance에서 가져온 코스튬 정보를 바탕으로 세팅
	UFallGlobal::ChangeCostumeColor(this, CostumeColor);
	UFallGlobal::ChangeCostumeTop(this, UpComp, CostumeTop);
	UFallGlobal::ChangeCostumeBot(this, LowComp, CostumeBot);
}

// Called every frame
void AEndPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEndPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

