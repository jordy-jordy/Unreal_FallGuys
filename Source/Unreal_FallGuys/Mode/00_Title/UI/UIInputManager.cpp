// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/UIInputManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Global/FallGlobal.h"
#include "Unreal_FallGuys.h"
#include "Mode/00_Title/TitlePlayerController.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"


// Sets default values for this component's properties
UUIInputManager::UUIInputManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UUIInputManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UUIInputManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUIInputManager::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(_PlayerInputComponent);

	ATitlePlayerController* PlayerController = Cast<ATitlePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	const UInputAction* UIMenuInputAction = PlayerController->GetInputAction(TEXT("IA_UIMenuInput"));
	const UInputAction* UIMoveInputAction = PlayerController->GetInputAction(TEXT("IA_UIMoveInput"));
	const UInputAction* UISelectInputAction = PlayerController->GetInputAction(TEXT("IA_UISelectInput"));

	//EnhancedInputComponent->BindAction(UIMenuInputAction, ETriggerEvent::Completed, this, &UUIInputManager::SwitchMenuWidget);
}

//void UUIInputManager::SwitchMenuWidget(const FInputActionValue& _Value)
//{
//	FVector2D Value = _Value.Get<FVector2D>();
//
//	UTitleMainWidget* Widget = UFallGlobal::GetMainWidget(GetWorld());
//
//	if (nullptr == Widget)
//	{
//#ifdef WITH_EDITOR
//		UE_LOG(FALL_DEV_LOG, Fatal, TEXT("%S(%u)> if (nullptr == Widget)"), __FUNCTION__, __LINE__);
//#endif
//		return;
//	}
//
//	ETitleUIType CurUIType = Widget->GetCurUIType();
//
//	if (Value.Y == 0)
//	{
//		if (Value.X > 0)
//		{
//			switch (CurUIType)
//			{
//			case ETitleUIType::TitleHome:
//				Widget->SwitchWidget(ETitleUIType::TitleCustom);
//				return;
//				break;
//			default:
//				break;
//			}
//		}
//		else if (Value.X < 0)
//		{
//			switch (CurUIType)
//			{
//			case ETitleUIType::TitleCustom:
//				Widget->SwitchWidget(ETitleUIType::TitleHome);
//				return;
//				break;
//			default:
//				break;
//			}
//		}
//	}
//	else if (Value.X == 0)
//	{
//		if (Value.Y > 0)
//		{
//			switch (CurUIType)
//			{
//			case ETitleUIType::TitleHome:
//				Widget->SwitchWidget(ETitleUIType::TitleEntrance);
//				return;
//				break;
//			case ETitleUIType::TitleCustom:
//				return;
//				break;
//			case ETitleUIType::TitleEntrance:
//				return;
//				break;
//			case ETitleUIType::CustomeInven:
//				return;
//				break;
//			case ETitleUIType::TitleIPPort:
//				// ServerConnect
//				return;
//				break;
//			case ETitleUIType::TitleName:
//				// SetNickName
//				Widget->SwitchWidget(ETitleUIType::TitleHome);
//				return;
//				break;
//			default:
//				break;
//			}
//		}
//		else if (Value.Y < 0)
//		{
//			switch (CurUIType)
//			{
//			case ETitleUIType::TitleCustom:
//				Widget->SwitchWidget(ETitleUIType::TitleHome);
//				return;
//				break;
//			case ETitleUIType::TitleEntrance:
//				Widget->SwitchWidget(ETitleUIType::TitleHome);
//				return;
//				break;
//			case ETitleUIType::CustomeInven:
//				//Widget->SwitchWidget(EUIType::TitleHome);
//				Widget->SwitchWidget(ETitleUIType::TitleCustom);
//				return;
//				break;
//			case ETitleUIType::TitleIPPort:
//				Widget->SwitchWidget(ETitleUIType::TitleEntrance);
//				return;
//				break;
//			default:
//				break;
//			}
//		}
//	}
//}


