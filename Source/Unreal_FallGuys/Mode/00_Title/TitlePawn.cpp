// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitlePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Global/BaseGameInstance.h>
#include <Mode/00_Title/UI/UIInputManager.h>
#include "Global/Data/GlobalDataTable.h"
#include <Global/FallGlobal.h>


// Sets default values
ATitlePawn::ATitlePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootSceneComp;
	UpComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Up"));
	UpComp->SetupAttachment(RootComponent);
	LowComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Low"));
	LowComp->SetupAttachment(RootComponent);

	UIInputManager = CreateDefaultSubobject<UUIInputManager>(TEXT("UIInputManager"));
}

// Called when the game starts or when spawned
void ATitlePawn::BeginPlay()
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
void ATitlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATitlePawn::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(_PlayerInputComponent);

	EnhancedInputComponent->BindActionValueLambda(PawnRotate, ETriggerEvent::Triggered,
		[this](const FInputActionValue& _Value)
		{
			// input is a Vector2D
			FVector2D LookAxisVector = _Value.Get<FVector2D>();

			if (Controller != nullptr)
			{
				// add yaw and pitch input to controller
				AddControllerYawInput(LookAxisVector.X);
				AddControllerPitchInput(LookAxisVector.Y);
			}
		});

	UIInputManager->SetupPlayerInputComponent(_PlayerInputComponent);
}

void ATitlePawn::PawnRotation(UStaticMeshComponent* _Target, const FVector2D& _Value)
{
#ifdef WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%S(%u)> %s"), __FUNCTION__, __LINE__, *_Value.ToString()));
#endif
	if (_Value.X < 0)
	{
		_Target->AddLocalRotation(FRotator(0.0f, 1.0f, 0.0f));
	}
	else if (_Value.X > 0)
	{
		_Target->AddLocalRotation(FRotator(0.0f, -1.0f, 0.0f));
	}
}
void ATitlePawn::AttachCustomStaticMesh(ECostumeType Type, FString& _ImgName)
{
	// _ImgName 에 해당하는 리소스 가져오기

	UBaseGameInstance* GameIns = GetGameInstance<UBaseGameInstance>();


	const FCostumeDataRow* CostumeData = UGlobalDataTable::GetCostumeData(GetWorld(), _ImgName);
	if (CostumeData && CostumeData->CostumeMesh)
	{
		switch (Type)
		{
		case ECostumeType::NONE:
			break;
		case ECostumeType::TOP:
			UpComp->SetStaticMesh(CostumeData->CostumeMesh);
			UFallGlobal::ChangeCostumeTop(this, UpComp,_ImgName);

			break;
		case ECostumeType::BOTTOM:
			LowComp->SetStaticMesh(CostumeData->CostumeMesh);
			UFallGlobal::ChangeCostumeBot(this, LowComp, _ImgName);
	
			break;
		case ECostumeType::MAX:
			break;
		default:
			break;
		}
	}


}

void ATitlePawn::DeAttachCustomStaticMesh(ECostumeType Type, FString& _ImgName)
{
	//UBaseGameInstance* GameIns = GetGameInstance<UBaseGameInstance>();


	//const FCostumeDataRow* CostumeData = UGlobalDataTable::GetCostumeData(GetWorld(), _ImgName);
	
		switch (Type)
		{
		case ECostumeType::NONE:
			break;
		case ECostumeType::TOP:
			UpComp->SetStaticMesh(nullptr);
			UFallGlobal::ChangeCostumeTop(this, UpComp, "");
			//CurUpStaticMesh = nullptr;
			break;
		case ECostumeType::BOTTOM:
			LowComp->SetStaticMesh(nullptr);
			UFallGlobal::ChangeCostumeBot(this, LowComp, "");

			//CurLowStaticMesh = nullptr;
			break;
		case ECostumeType::MAX:
			break;
		default:
			break;
		}

}

