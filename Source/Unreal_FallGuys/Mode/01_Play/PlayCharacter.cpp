// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/BaseGameInstance.h>
#include "Net/UnrealNetwork.h"


// Sets default values
APlayCharacter::APlayCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void APlayCharacter::S2M_Costume_Implementation(const FString& _Name)
{
	GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeMesh(this, _Name));
	CName = _Name;
}

void APlayCharacter::C2S_Costume_Implementation(const FString& _Name)
{
	GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeMesh(this, _Name));
	CName = _Name;
	S2M_Costume(CName);
}

// Called when the game starts or when spawned
void APlayCharacter::BeginPlay()
{
	Super::BeginPlay();

	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(GetGameInstance());
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == GetController())
	{
		// 나는 그냥 내 코스츔 하면 된다.
		CName = UFallGlobal::GetCostumeName(this);
		GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeMesh(this, CName));
		C2S_Costume(CName);
	}
	else
	{
		GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeMesh(this, CName));
	}
}

// Called every frame
void APlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector APlayCharacter::GetControllerForward()
{
	const FRotator Rotaion = Controller->GetControlRotation();
	const FRotator YawRoation = FRotator(0.0f, Rotaion.Yaw, 0.0f);
	return FRotationMatrix(YawRoation).GetUnitAxis(EAxis::X);
}

FVector APlayCharacter::GetControllerRight()
{
	const FRotator Rotaion = Controller->GetControlRotation();
	const FRotator YawRoation = FRotator(0.0f, Rotaion.Yaw, 0.0f);
	return FRotationMatrix(YawRoation).GetUnitAxis(EAxis::Y);
}

// Called to bind functionality to input
void APlayCharacter::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(_PlayerInputComponent);

	EnhancedInputComponent->BindActionValueLambda(LookAction, ETriggerEvent::Triggered,
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

void APlayCharacter::PlayerWMove()
{
	AddMovementInput(GetControllerForward());
}
void APlayCharacter::PlayerSMove()
{
	AddMovementInput(-GetControllerForward());
}
void APlayCharacter::PlayerDMove()
{
	AddMovementInput(GetControllerRight());
}
void APlayCharacter::PlayerAMove()
{
	AddMovementInput(-GetControllerRight());
}

void APlayCharacter::TestMove(const FVector2D& _Value)
{
	const FRotator Rotaion = Controller->GetControlRotation();

	const FRotator YawRoation = FRotator(0.0f, Rotaion.Yaw, 0.0f);

	const FVector Forward = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, _Value.X);
	AddMovementInput(Right, _Value.Y);
}


void APlayCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayCharacter, CName);
}