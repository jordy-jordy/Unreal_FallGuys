// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"
#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/BaseGameInstance.h>


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

	CoustumeTOPStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TOPMesh"));
	CoustumeBOTStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOTMesh"));
	CoustumeTOPStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CoustumeBOTStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CoustumeTOPStaticMesh->SetGenerateOverlapEvents(false);
	CoustumeBOTStaticMesh->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void APlayCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 스켈레탈 메시 소켓에 어태치
	if (CoustumeTOPStaticMesh && CoustumeBOTStaticMesh)
	{
		CoustumeTOPStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HeadSocket"));
		CoustumeBOTStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("CustomSocket"));
	}

	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(GetGameInstance());

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == GetController())
	{
		CostumeColor = UFallGlobal::GetCostumeColor(this);
		CostumeTopName = UFallGlobal::GetCostumeTop(this);
		CostumeBotName = UFallGlobal::GetCostumeBot(this);
		GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, CostumeColor));
		CoustumeTOPStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeTopName));
		CoustumeBOTStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeBotName));
		
		C2S_Costume(CostumeColor, CostumeTopName, CostumeBotName);
	}
	else
	{
		GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, CostumeColor));
		CoustumeTOPStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeTopName));
		CoustumeBOTStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeBotName));
	}

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == GetController())
	{
		IsDie = GameIns->GetIsDie();
		C2S_IsDie(IsDie);
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

	DOREPLIFETIME(APlayCharacter, CostumeColor);
	DOREPLIFETIME(APlayCharacter, CostumeTopName);
	DOREPLIFETIME(APlayCharacter, CostumeBotName);
	DOREPLIFETIME(APlayCharacter, IsDie);
}

void APlayCharacter::C2S_IsDie_Implementation(bool _val)
{
	IsDie = _val;
	S2M_IsDie(IsDie);
}

// 이현정 : 캐릭터 상태 동기화를 위한 함수
void APlayCharacter::S2M_IsDie_Implementation(bool _val)
{
	IsDie = _val;
}

// 이현정 : 캐릭터 코스튬 설정 - 클라 > 서버
void APlayCharacter::C2S_Costume_Implementation(const FString& _Color, const FString& _TopName, const FString& _BotName)
{
	CostumeColor = _Color;
	CostumeTopName = _TopName;
	CostumeBotName = _BotName;
	GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, _Color));
	CoustumeTOPStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, _TopName));
	CoustumeBOTStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, _BotName));
	S2M_Costume(CostumeColor, CostumeTopName, CostumeBotName);
}

// 이현정 : 캐릭터 코스튬 설정 - 서버 > 멀티캐스트
void APlayCharacter::S2M_Costume_Implementation(const FString& _Color, const FString& _TopName, const FString& _BotName)
{
	CostumeColor = _Color;
	CostumeTopName = _TopName;
	CostumeBotName = _BotName;
	GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, _Color));
	CoustumeTOPStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, _TopName));
	CoustumeBOTStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, _BotName));
}





