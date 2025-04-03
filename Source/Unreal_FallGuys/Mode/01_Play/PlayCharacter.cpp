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

	CostumeTOPStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TOPMesh"));
	CostumeTOPStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	CostumeTOPStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CostumeTOPStaticMesh->SetGenerateOverlapEvents(false);
	CostumeTOPStaticMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	CostumeBOTStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOTMesh"));
	CostumeBOTStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	CostumeBOTStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CostumeBOTStaticMesh->SetGenerateOverlapEvents(false);
	CostumeBOTStaticMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
}

// Called when the game starts or when spawned
void APlayCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 스켈레탈 메시 소켓에 어태치
	if (CostumeTOPStaticMesh && CostumeBOTStaticMesh)
	{
		CostumeTOPStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HeadSocket"));
		CostumeBOTStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("CustomSocket"));
	}

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == GetController())
	{
		CostumeColor = UFallGlobal::GetCostumeColor(this);
		CostumeTopName = UFallGlobal::GetCostumeTop(this);
		CostumeBotName = UFallGlobal::GetCostumeBot(this);

		if (CostumeColor != TEXT(""))
		{
			GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, CostumeColor));
		}

		if (CostumeTopName != TEXT(""))
		{
			CostumeTOPStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeTopName));
		}

		if (CostumeBotName != TEXT(""))
		{
			CostumeBOTStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeBotName));
		}

		C2S_Costume(CostumeColor, CostumeTopName, CostumeBotName);
	}
	else
	{
		if (CostumeColor != TEXT(""))
		{
			GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, CostumeColor));
		}

		if (CostumeTopName != TEXT(""))
		{
			CostumeTOPStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeTopName));
		}

		if (CostumeBotName != TEXT(""))
		{
			CostumeBOTStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeBotName));
		}
	}

	// 서버장일 경우: 직접 PlayerState에 Status 설정
	if (HasAuthority()) // 서버만 실행
	{
		UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(GetGameInstance());
		IsDie = GameIns->GetIsDie();

		APlayPlayerState* PS = GetPlayerState<APlayPlayerState>();
		if (PS != nullptr)
		{
			PS->PlayerInfo.Status = IsDie ? EPlayerStatus::FAIL : EPlayerStatus::SUCCESS;
		}
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
	if (!CanMove) { return; }
	AddMovementInput(GetControllerForward());
}
void APlayCharacter::PlayerSMove()
{
	if (!CanMove) { return; }
	AddMovementInput(-GetControllerForward());
}
void APlayCharacter::PlayerDMove()
{
	if (!CanMove) { return; }
	AddMovementInput(GetControllerRight());
}
void APlayCharacter::PlayerAMove()
{
	if (!CanMove) { return; }
	AddMovementInput(-GetControllerRight());
}

void APlayCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayCharacter, CostumeColor);
	DOREPLIFETIME(APlayCharacter, CostumeTopName);
	DOREPLIFETIME(APlayCharacter, CostumeBotName);
	DOREPLIFETIME(APlayCharacter, IsDie);
	DOREPLIFETIME(APlayCharacter, CanMove);
}

void APlayCharacter::C2S_IsDie_Implementation(bool _val)
{
	IsDie = _val;
	
	APlayPlayerState* PS = GetPlayerState<APlayPlayerState>();
	if (IsDie == false)
	{
		PS->PlayerInfo.Status = EPlayerStatus::SUCCESS;
	}
	else
	{
		PS->PlayerInfo.Status = EPlayerStatus::FAIL;
	}

	S2M_IsDie(IsDie);
}

// 이현정 : 캐릭터 상태 동기화를 위한 함수
void APlayCharacter::S2M_IsDie_Implementation(bool _val)
{
	IsDie = _val;

	APlayPlayerState* PS = GetPlayerState<APlayPlayerState>();
	if (IsDie == false)
	{
		PS->PlayerInfo.Status = EPlayerStatus::SUCCESS;
	}
	else
	{
		PS->PlayerInfo.Status = EPlayerStatus::FAIL;
	}
}

// 이현정 : 캐릭터 코스튬 설정 - 클라 > 서버
void APlayCharacter::C2S_Costume_Implementation(const FString& _Color, const FString& _TopName, const FString& _BotName)
{
	CostumeColor = _Color;
	CostumeTopName = _TopName;
	CostumeBotName = _BotName;

	if (CostumeColor != TEXT(""))
	{
		GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, CostumeColor));
	}

	if (CostumeTopName != TEXT(""))
	{
		CostumeTOPStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeTopName));
	}

	if (CostumeBotName != TEXT(""))
	{
		CostumeBOTStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeBotName));
	}

	S2M_Costume(CostumeColor, CostumeTopName, CostumeBotName);
}

// 이현정 : 캐릭터 코스튬 설정 - 서버 > 멀티캐스트
void APlayCharacter::S2M_Costume_Implementation(const FString& _Color, const FString& _TopName, const FString& _BotName)
{
	CostumeColor = _Color;
	CostumeTopName = _TopName;
	CostumeBotName = _BotName;

	if (CostumeColor != TEXT(""))
	{
		GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, CostumeColor));
	}

	if (CostumeTopName != TEXT(""))
	{
		CostumeTOPStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeTopName));
	}

	if (CostumeBotName != TEXT(""))
	{
		CostumeBOTStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, CostumeBotName));
	}
}

// 이현정 : 캐릭터 Moving 활성화
void APlayCharacter::S2M_SetCanMoveTrue_Implementation()
{
	CanMove = true;
}

// 이현정 : 캐릭터 Moving 비활성화
void APlayCharacter::S2M_SetCanMoveFalse_Implementation()
{
	CanMove = false;
}

// 이현정 : 클라이언트 플레이어의 상태 동기화
void APlayCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	APlayPlayerState* PS = GetPlayerState<APlayPlayerState>();
	if (!IsValid(PS))
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("OnRep_PlayerState :: PS가 nullptr입니다."));
		return;
	}

	// GameInstance에서 IsDie 가져와서 서버에 전달
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(GetGameInstance());
	if (GameIns)
	{
		IsDie = GameIns->GetIsDie();
		C2S_IsDie(IsDie); // 클라 → 서버
	}
}
