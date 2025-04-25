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
	DOREPLIFETIME(APlayCharacter, bIsSpectar);
	DOREPLIFETIME(APlayCharacter, bIsResultLevel);
}

// Called when the game starts or when spawned
void APlayCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 이현정 : 캐릭터 무브먼트 컴포넌트 세팅
	// 캐릭터 무브먼트 :: 일반 세팅 : 마찰 인수 제동
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	// 캐릭터 무브먼트 :: 일반 세팅 : 최대 가속
	GetCharacterMovement()->MaxAcceleration = 2500.0f;
	// 캐릭터 무브먼트 :: 걷기 : 지면 마찰
	GetCharacterMovement()->GroundFriction = 0.8f;
	// 캐릭터 무브먼트 :: 걷기 : 최대 걷기 속도
	GetCharacterMovement()->MaxWalkSpeed = 510.0f;
	// 캐릭터 무브먼트 :: 걷기 : 감속 걷기 제동
	GetCharacterMovement()->BrakingDecelerationWalking = 3500.0f;
	// 캐릭터 무브먼트 :: 점프/낙하 : 대기 컨트롤
	GetCharacterMovement()->AirControl = 0.4f;

	// 스켈레탈 메시 소켓에 어태치
	if (CostumeTOPStaticMesh && CostumeBOTStaticMesh)
	{
		CostumeTOPStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HeadSocket"));
		CostumeBOTStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("CustomSocket"));
	}

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == GetController())
	{
		// 코스튬 세팅
		CostumeColor = UFallGlobal::GetCostumeColor(this);
		CostumeTopName = UFallGlobal::GetCostumeTop(this);
		CostumeBotName = UFallGlobal::GetCostumeBot(this);
		SetCharacterCostume(CostumeColor, CostumeTopName, CostumeBotName);
		C2S_Costume(CostumeColor, CostumeTopName, CostumeBotName);

		// 닉네임 세팅
		NickName = UFallGlobal::GetNickname(this);
		C2S_NickName(NickName);
	}
	else
	{
		// 코스튬 세팅
		SetCharacterCostume(CostumeColor, CostumeTopName, CostumeBotName);
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

// 이현정 : 캐릭터 코스튬 설정 - 로컬
void APlayCharacter::SetCharacterCostume(FString _Color, FString _Top, FString _Bot)
{
	if (_Color != TEXT(""))
	{
		GetMesh()->SetSkeletalMesh(UFallGlobal::GetCostumeColorMesh(this, _Color));
	}

	if (_Top != TEXT(""))
	{
		CostumeTOPStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, _Top));
	}

	if (_Bot != TEXT(""))
	{
		CostumeBOTStaticMesh->SetStaticMesh(UFallGlobal::GetCostumeMesh(this, _Bot));
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

void APlayCharacter::InitializeFromPlayerInfo(const FPlayerInfo& _Info)
{
	// 중복된 태그 방지
	if (!Tags.Contains(_Info.Tag))
	{
		Tags.Add(_Info.Tag);
	}

	// PlayerState의 Status에 따라 IsDie 세팅
	CurStatus = _Info.Status;
	IsDie = (CurStatus == EPlayerStatus::FAIL);

	// 관전자 상태 세팅
	bIsSpectar = _Info.bIsSpectar;

	DebugCheckDieStatus();
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
			InitializeFromPlayerInfo(PlayState->PlayerInfo);
		}
	}

	//// PlayerState의 Status에 따라 IsDie 세팅
	//CurStatus = PlayState->GetPlayerStateStatus();
	//IsDie = (CurStatus == EPlayerStatus::FAIL);

	//// 관전자 상태 세팅
	//bIsSpectar = IsDie;
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
			InitializeFromPlayerInfo(PlayState->PlayerInfo);
		}
	}
	//// PlayerState의 Status에 따라 IsDie 세팅
	//IsDie = (CurStatus == EPlayerStatus::FAIL);

	//// 관전자 상태 세팅
	//bIsSpectar = IsDie;
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
			TagStringForLog += Tag.ToString() + TEXT("");
		}
	}
	else
	{
		TagStringForLog = TEXT("태그 없음");
	}

	UE_LOG(FALL_DEV_LOG, Log, TEXT("Tag: %s / CurStatus: %s / IsDie: %s / Spectar: %s"),
		*TagStringForLog,
		*StatusStr,
		IsDie ? TEXT("true") : TEXT("false"),
		bIsSpectar ? TEXT("true") : TEXT("false"));

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

void APlayCharacter::C2S_NickName_Implementation(const FString& _NickName)
{
	NickName = _NickName;
	//S2M_NickName(NickName);
}

void APlayCharacter::S2M_NickName_Implementation(const FString& _NickName)
{
	NickName = _NickName;
}

void APlayCharacter::S2M_ActivateSpectatorMode_Implementation()
{
	if (IsLocallyControlled())
	{
		SpectatorOn(); // 클라에서만 관전자 모드 실행
	}
}

void APlayCharacter::ApplySpectatorVisibility()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetEnableGravity(false);
	SetActorLocation(FVector(0, 0, -100000));

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("ApplySpectatorVisibility :: 관전자 숨김 처리 완료"));
}

void APlayCharacter::S2M_ApplySpectatorVisibility_Implementation(bool _bIsSpectar)
{
	if (bSpectatorApplied) return;
	ApplySpectatorVisibility();  // 숨김 처리
	S2M_ActivateSpectatorMode(); // 관전자 모드 진입 (클라 포함 모두 적용)
	bSpectatorApplied = true;	 // 중복 방지 설정
}
