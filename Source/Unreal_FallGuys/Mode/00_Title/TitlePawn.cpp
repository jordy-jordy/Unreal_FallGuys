// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitlePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Global/BaseGameInstance.h>
#include "Global/Data/GlobalDataTable.h"


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

}

// Called when the game starts or when spawned
void ATitlePawn::BeginPlay()
{
	Super::BeginPlay();

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
}

void ATitlePawn::PawnRotation(UStaticMeshComponent* _Target, const FVector2D& _Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%S(%u)> %s"), __FUNCTION__, __LINE__, *_Value.ToString()));

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
			CurUpStaticMesh = CostumeData->CostumeMesh;
			break;
		case ECostumeType::BOTTOM:
			LowComp->SetStaticMesh(CostumeData->CostumeMesh);
			CurLowStaticMesh = CostumeData->CostumeMesh;
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
			CurUpStaticMesh = nullptr;
			break;
		case ECostumeType::BOTTOM:
			LowComp->SetStaticMesh(nullptr);
			CurLowStaticMesh = nullptr;
			break;
		case ECostumeType::MAX:
			break;
		default:
			break;
		}

}

