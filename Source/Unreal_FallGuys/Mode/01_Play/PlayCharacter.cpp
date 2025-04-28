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
	SpringArmComponent->bDoCollisionTest = false; // 충돌 검사 비활성화

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
	DOREPLIFETIME(APlayCharacter, bSpectatorApplied);
	DOREPLIFETIME(APlayCharacter, bVisibilityApplied);
	DOREPLIFETIME(APlayCharacter, ReplicatedCameraRotation);
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

	// 자기 자신의 카메라 회전 복제
	if (IsLocallyControlled())
	{
		ReplicatedCameraRotation = GetControlRotation();
	}
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

void APlayCharacter::C2S_NickName_Implementation(const FString& _NickName)
{
	NickName = _NickName;
	//S2M_NickName(NickName);
}

void APlayCharacter::S2M_NickName_Implementation(const FString& _NickName)
{
	NickName = _NickName;
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

// 이현정 : 서버장의 캐릭터 상태를 세팅
void APlayCharacter::PossessedBy(AController* _NewController)
{
	Super::PossessedBy(_NewController);

	APlayPlayerState* PlayState = GetPlayerState<APlayPlayerState>();
	if (PlayState)
	{
		InitializeFromPlayerInfo(PlayState->PlayerInfo);
	}
}

// 이현정 : 클라이언트의 캐릭터 상태를 세팅
void APlayCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	APlayPlayerState* PlayState = GetPlayerState<APlayPlayerState>();
	if (PlayState)
	{
		InitializeFromPlayerInfo(PlayState->PlayerInfo);
	}
}

// PlayPlayerState 로 부터 정보를 세팅함
void APlayCharacter::InitializeFromPlayerInfo(const FPlayerInfo& _Info)
{
	// 중복된 태그 방지
	Tags.Empty();
	Tags.Add(_Info.Tag);

	// PlayerState의 Status에 따라 IsDie 세팅
	CurStatus = _Info.Status;
	IsDie = (CurStatus == EPlayerStatus::FAIL);

	// 관전자 상태 세팅
	bIsSpectar = _Info.bIsSpectar;

	// 결과 화면에서 숨겨져야 하는지
	HiddenInResult = _Info.bCanHiddenAtResult;

	DebugCheckDieStatus();
}

// 플레이어를 투명화 : PlayGameMode로부터 호출됨 !!! 게임이 시작됐을때
void APlayCharacter::S2M_ApplySpectatorVisibility_Implementation()
{
	if (bVisibilityApplied) return;
	
	GetMovementComponent()->StopMovementImmediately();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetEnableGravity(false);

	TArray<UActorComponent*> Components;
	GetComponents(Components);
	for (UActorComponent* Comp : Components)
	{
		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Comp))
		{
			PrimComp->SetVisibility(false, true);
			PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	bVisibilityApplied = true;

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayCharacter :: 일반 스테이지 :: 관전자 숨김 처리 완료 :: 닉네임 : %s"),
	*NickName);
}

// 플레이어를 투명화 : PlayGameMode로부터 호출됨 !!! Goal Or Kill 콜리전에 닿았을때
void APlayCharacter::C2S_ApplySpectatorVisibilityAtGoalColl_Implementation()
{
	GetMovementComponent()->StopMovementImmediately();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetEnableGravity(false);

	TArray<UActorComponent*> Components;
	GetComponents(Components);
	for (UActorComponent* Comp : Components)
	{
		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Comp))
		{
			PrimComp->SetVisibility(false, true);
			PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	S2M_ApplySpectatorVisibilityAtGoalColl();
}

// 플레이어를 투명화 : PlayGameMode로부터 호출됨 !!! Goal Or Kill 콜리전에 닿았을때
void APlayCharacter::S2M_ApplySpectatorVisibilityAtGoalColl_Implementation()
{
	GetMovementComponent()->StopMovementImmediately();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetEnableGravity(false);

	TArray<UActorComponent*> Components;
	GetComponents(Components);
	for (UActorComponent* Comp : Components)
	{
		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Comp))
		{
			PrimComp->SetVisibility(false, true);
			PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("PlayCharacter :: GoalColl or KillZone 처리 :: 닉네임 : %s"),
		*NickName);
}

void APlayCharacter::C2S_RequestRandomView_Implementation()
{
	APlayGameMode* PlayMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PlayMode != nullptr && PC != nullptr)
	{
		PlayMode->SetRandomViewForClient(PC);
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayCharacter :: Server_RequestRandomView :: 서버에서 랜덤 뷰 요청 처리"));
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("PlayCharacter :: Server_RequestRandomView :: GameMode 또는 Controller가 nullptr"));
	}
}

void APlayCharacter::C2S_RequestSetViewByIndex_Implementation(int32 _TargetIndex)
{
	APlayGameMode* PlayMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PlayMode == nullptr)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("C2S_RequestSetViewByIndex :: PlayMode is nullptr"));
	}
	if (PC == nullptr)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("C2S_RequestSetViewByIndex :: Controller is nullptr"));
	}

	if (PlayMode != nullptr && PC != nullptr)
	{
		PlayMode->SetViewForClientByIndex(PC, _TargetIndex);
		UE_LOG(FALL_DEV_LOG, Log, TEXT("PlayCharacter :: C2S_RequestSetViewByIndex :: 인덱스 %d로 요청"), _TargetIndex);
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
			TagStringForLog += Tag.ToString() + TEXT("");
		}
	}
	else
	{
		TagStringForLog = TEXT("태그 없음");
	}

	UE_LOG(FALL_DEV_LOG, Log, TEXT("닉네임 : %s | 태그 : %s | 현재 상태 : %s | IsDie : %s | Spectar : %s | HiddenResult : %s"),
		*NickName,
		*TagStringForLog,
		*StatusStr,
		IsDie ? TEXT("TRUE") : TEXT("FALSE"),
		bIsSpectar ? TEXT("TRUE") : TEXT("FALSE"),
		HiddenInResult ? TEXT("TRUE") : TEXT("FALSE")
	);

	if (UFallConst::PrintDebugLog && GEngine)
	{
		FString TagStringForScreen;

		if (Tags.Num() > 0)
		{
			for (const FName& Tag : Tags)
			{
				TagStringForScreen += Tag.ToString() + TEXT("");
			}
		}
		else
		{
			TagStringForScreen = TEXT("태그 없음");
		}

		const FString ScreenMsg = FString::Printf(TEXT("[Debug] 닉네임 : %s | 태그 : %s | 현재 상태 : %s | IsDie : %s | Spectar : %s"),
			*NickName,
			*TagStringForScreen,
			*StatusStr,
			IsDie ? TEXT("TRUE") : TEXT("FALSE"),
			bIsSpectar ? TEXT("TRUE") : TEXT("FALSE"),
			HiddenInResult ? TEXT("TRUE") : TEXT("FALSE")
		);

		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, ScreenMsg);
	}
}



