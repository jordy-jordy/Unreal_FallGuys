// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/02_End/EndPawn.h"
#include "Net/UnrealNetwork.h"

#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/Data/GlobalDataTable.h>
#include <Global/BaseGameInstance.h>
#include "Mode/01_Play/PlayPlayerState.h"


// Sets default values
AEndPawn::AEndPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	UpComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Up"));
	LowComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Low"));
}

// Called when the game starts or when spawned
void AEndPawn::BeginPlay()
{
	Super::BeginPlay();

	// 최종 승리 플레이어의 정보를 가져오고 세팅
	UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	if (GI)
	{
		const FWinnerInfo& Info = GI->InsGetWinnerInfo();

		UE_LOG(FALL_DEV_LOG, Log, TEXT("EndPawn :: BeginPlay :: WinnerInfo 읽음 - %s"), *Info.NickName);

		InitDirectly(Info.NickName, Info.CostumeColor, Info.CostumeTop, Info.CostumeBot);
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("EndPawn :: BeginPlay :: GameInstance 없음"));
	}
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

// 게임 인스로부터 승리한 플레이어의 정보를 얻어옴
void AEndPawn::InitDirectly(const FString& _Nick, const FString& _Color, const FString& _Top, const FString& _Bot)
{
	NickName = _Nick;
	CostumeColor = _Color;
	CostumeTop = _Top;
	CostumeBot = _Bot;

	UE_LOG(FALL_DEV_LOG, Log, TEXT("EndPawn :: InitDirectly :: 닉네임: %s, 컬러: %s, 상의: %s, 하의: %s"),
		*_Nick, *_Color, *_Top, *_Bot);

	UpComp->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("HeadSocket"));
	LowComp->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("CustomSocket"));

	UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	if (GI)
	{
		if (CostumeColor != TEXT(""))
		{
			BaseMesh->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, CostumeColor));
		}
		if (CostumeTop != TEXT(""))
		{
			UpComp->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeTop));
		}
		if (CostumeBot != TEXT(""))
		{
			LowComp->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeBot));
		}
	}
}
