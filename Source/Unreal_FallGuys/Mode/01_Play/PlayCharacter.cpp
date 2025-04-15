// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include <Net/UnrealNetwork.h>
#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/FallConst.h>
#include <Global/GlobalEnum.h>
#include <Global/BaseGameInstance.h>
#include <Mode/01_Play/PlayGameMode.h>
#include <Mode/01_Play/PlayPlayerState.h>


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

	bReplicates = true;
}

void APlayCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayCharacter, CurAnimnation);
	DOREPLIFETIME(APlayCharacter, CostumeColor);
	DOREPLIFETIME(APlayCharacter, CostumeTopName);
	DOREPLIFETIME(APlayCharacter, CostumeBotName);
	DOREPLIFETIME(APlayCharacter, NickName);
	DOREPLIFETIME(APlayCharacter, IsDie);
	DOREPLIFETIME(APlayCharacter, CanMove);
	DOREPLIFETIME(APlayCharacter, CurStatus);
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

		NickName = UFallGlobal::GetNickname(this);
		C2S_Costume(CostumeColor, CostumeTopName, CostumeBotName);
		C2S_NickName(NickName);
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
}

// Called every frame
void APlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Test = GetWorld()->GetAuthGameMode();

	this;

	NickName;
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

void APlayCharacter::C2S_NickName_Implementation(const FString& _NickName)
{

	NickName = _NickName;

	//S2M_NickName(NickName);
}

void APlayCharacter::S2M_NickName_Implementation(const FString& _NickName)
{
	NickName = _NickName;
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
// 이현정 : 서버장의 캐릭터 상태를 세팅
void APlayCharacter::PossessedBy(AController* _NewController)
{
	Super::PossessedBy(_NewController);

	APlayPlayerState* PlayState = GetPlayerState<APlayPlayerState>();
	if (PlayState)
	{
		// PlayerState에 있는 Tag를 캐릭터 태그에 세팅
		if (!PlayState->PlayerInfo.Tag.IsNone())
		{
			Tags.Add(FName(PlayState->PlayerInfo.Tag));
		}

		// PlayerState의 Status에 따라 IsDie 세팅
		CurStatus = PlayState->GetPlayerStateStatus();
		IsDie = (CurStatus == EPlayerStatus::FAIL);
		DebugCheckDieStatus(); // 상태 디버깅
	}
}

// 이현정 : 클라이언트의 캐릭터 상태를 세팅
void APlayCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	APlayPlayerState* PlayState = GetPlayerState<APlayPlayerState>();
	if (PlayState)
	{
		// PlayerState에 있는 Tag를 캐릭터 태그에 세팅
		if (!PlayState->PlayerInfo.Tag.IsNone())
		{
			Tags.Add(FName(PlayState->PlayerInfo.Tag));
		}

		// PlayerState의 Status에 따라 IsDie 세팅
		CurStatus = PlayState->GetPlayerStateStatus();
		IsDie = (CurStatus == EPlayerStatus::FAIL);
		DebugCheckDieStatus(); // 상태 디버깅
	}
}

// 이현정 : 디버그용 : 캐릭터 상태 확인
void APlayCharacter::DebugCheckDieStatus()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("========== 캐릭터 상태 디버그 =========="));
	FString StatusStr = UEnum::GetValueAsString(CurStatus);
	FString TagStringForLog;

	if (Tags.Num() > 0)
	{
		for (const FName& Tag : Tags)
		{
			TagStringForLog += Tag.ToString() + TEXT(" ");
		}
	}
	else
	{
		TagStringForLog = TEXT("태그 없음");
	}

	UE_LOG(FALL_DEV_LOG, Log, TEXT("Tag: %s / CurStatus: %s / IsDie: %s"),
		*TagStringForLog,
		*StatusStr,
		IsDie ? TEXT("true") : TEXT("false"));

	if (UFallConst::PrintDebugLog && GEngine)
	{
		FString TagStringForScreen;

		if (Tags.Num() > 0)
		{
			for (const FName& Tag : Tags)
			{
				TagStringForScreen += Tag.ToString() + TEXT(" ");
			}
		}
		else
		{
			TagStringForScreen = TEXT("태그 없음");
		}

		const FString ScreenMsg = FString::Printf(TEXT("[Debug] Tag: %s / CurStatus: %s / IsDie: %s"),
			*TagStringForScreen,
			*StatusStr,
			IsDie ? TEXT("true") : TEXT("false"));

		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, ScreenMsg);
	}
}

