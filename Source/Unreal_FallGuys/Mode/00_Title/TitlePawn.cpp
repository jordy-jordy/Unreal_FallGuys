// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/TitlePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
ATitlePawn::ATitlePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ATitlePawn::PawnRotation(const FVector2D& _Value)
{
	//const FRotator Rotaion = Controller->GetControlRotation();
	//// z축만 뽑아냅니다.
	//const FRotator YawRoation = FRotator(0.0f, Rotaion.Yaw, 0.0f);

	//// 회전행렬
	//const FVector Forward = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::X);
	//const FVector Right = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::Y);
	//const FVector Up = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::Z);

	//AddMovementInput(Forward, _Value.X);
	//AddMovementInput(Right, _Value.Y);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%S(%u)> %s"), __FUNCTION__, __LINE__, *_Value.ToString()));

	const FRotator Rotation = Controller->GetControlRotation();

	if (_Value.X < 0)
	{
		AddActorLocalRotation(FRotator(0.0f, -1.0f, 0.0f));
	}
	else if (_Value.X > 0)
	{
		AddActorLocalRotation(FRotator(0.0f, 1.0f, 0.0f));
	}
}


