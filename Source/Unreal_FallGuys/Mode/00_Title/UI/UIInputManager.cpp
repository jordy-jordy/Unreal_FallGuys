// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/UIInputManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Global/FallGlobal.h"
#include "Unreal_FallGuys.h"
#include "Mode/00_Title/TitlePlayerController.h"

#include "Mode/00_Title/UI/TitleMainWidget.h"
#include "Mode/00_Title/UI/TitleHomeWidget.h"
#include "Mode/00_Title/UI/TitleMenuWidget.h"
#include "Mode/00_Title/UI/TitleEntranceWidget.h"
#include "Mode/00_Title/UI/TitlePlayerCountWidget.h"
#include "Mode/00_Title/UI/TitleIPPortWidget.h"
#include "Mode/00_Title/UI/TitleCustomizingMainWidget.h"
#include "Mode/00_Title/UI/TitleCustomWidget.h"


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

	// Select
	const UInputAction* UI_Space = PlayerController->GetInputAction(TEXT("IA_UI_Space"));
	const UInputAction* UI_Esc = PlayerController->GetInputAction(TEXT("IA_UI_Esc"));

	EnhancedInputComponent->BindAction(UI_Space, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_SPACE);
	EnhancedInputComponent->BindAction(UI_Esc, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_ESC);

	// Menu
	const UInputAction* UI_Q = PlayerController->GetInputAction(TEXT("IA_UI_Q"));
	const UInputAction* UI_E = PlayerController->GetInputAction(TEXT("IA_UI_E"));

	EnhancedInputComponent->BindAction(UI_Q, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_Q);
	EnhancedInputComponent->BindAction(UI_E, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_E);

	// Move
	const UInputAction* UI_W = PlayerController->GetInputAction(TEXT("IA_UI_W"));
	const UInputAction* UI_A = PlayerController->GetInputAction(TEXT("IA_UI_A"));
	const UInputAction* UI_S = PlayerController->GetInputAction(TEXT("IA_UI_S"));
	const UInputAction* UI_D = PlayerController->GetInputAction(TEXT("IA_UI_D"));

	EnhancedInputComponent->BindAction(UI_W, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_W);
	EnhancedInputComponent->BindAction(UI_A, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_A);
	EnhancedInputComponent->BindAction(UI_S, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_S);
	EnhancedInputComponent->BindAction(UI_D, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_D);
}

// Select
void UUIInputManager::SwitchWidget_SPACE()
{
	UTitleMainWidget* MainWidget = UFallGlobal::GetMainWidget(GetWorld());
	UTitleHomeWidget* HomeWidget = MainWidget->FindWidget<UTitleHomeWidget>(ETitleUIType::TitleHome);
	UTitleEntranceWidget* EntranceWidget = MainWidget->FindWidget<UTitleEntranceWidget>(ETitleUIType::TitleEntrance);

	if (nullptr == MainWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : MainWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == HomeWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : HomeWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == EntranceWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : EntranceWidget is null"), *FString(__FUNCSIG__));
		return;
	}

	ETitleUIType CurType = MainWidget->GetCurUIType();
	if (ETitleUIType::TitleHome == CurType)
	{
		HomeWidget->SwitchToEntrance();
		return;
	}
	else if (ETitleUIType::TitleEntrance == CurType)
	{
		EntranceWidget->KeyInputButtonChoice();
	}
}

void UUIInputManager::SwitchWidget_ESC()
{
	UTitleMainWidget* MainWidget = UFallGlobal::GetMainWidget(GetWorld());
	UTitleEntranceWidget* EntranceWidget = MainWidget->FindWidget<UTitleEntranceWidget>(ETitleUIType::TitleEntrance);
	UTitlePlayerCountWidget* PlayerCountWidget = MainWidget->FindWidget<UTitlePlayerCountWidget>(ETitleUIType::TitlePlayerCount);
	UTitleIPPortWidget* IPPortWidget = MainWidget->FindWidget<UTitleIPPortWidget>(ETitleUIType::TitleIPPort);
	UTitleCustomizingMainWidget* CustomizingMainWidget = MainWidget->FindWidget<UTitleCustomizingMainWidget>(ETitleUIType::TitleCustom);
	UTitleCustomWidget* CustomInvenWidget = MainWidget->FindWidget<UTitleCustomWidget>(ETitleUIType::CustomeInven);

	if (nullptr == MainWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : MainWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == EntranceWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : EntranceWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == PlayerCountWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : PlayerCountWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == IPPortWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : IPPortWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == CustomizingMainWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : CustomizingMainWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == CustomInvenWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : CustomInvenWidget is null"), *FString(__FUNCSIG__));
		return;
	}

	ETitleUIType CurType = MainWidget->GetCurUIType();
	if (ETitleUIType::TitleEntrance == CurType)
	{
		EntranceWidget->SwtichToHome();
		return;
	}
	else if (ETitleUIType::TitlePlayerCount == CurType)
	{
		PlayerCountWidget->SwitchToEntrance();
		return;
	}
	else if (ETitleUIType::TitleIPPort == CurType)
	{
		IPPortWidget->SwitchToEntrance();
		return;
	}
	else if (ETitleUIType::TitleCustom == CurType)
	{
		CustomizingMainWidget->SwitchToHome();
		return;
	}
	else if (ETitleUIType::CustomeInven == CurType)
	{
		CustomInvenWidget->SwitchToCustomize();
		return;
	}
}

// Menu
void UUIInputManager::SwitchWidget_Q()
{
	UTitleMainWidget* MainWidget = UFallGlobal::GetMainWidget(GetWorld());
	UTitleMenuWidget* MenuWidget = MainWidget->FindWidget<UTitleMenuWidget>(ETitleUIType::TitleMenu);
	UTitleEntranceWidget* EntranceWidget = MainWidget->FindWidget<UTitleEntranceWidget>(ETitleUIType::TitleEntrance);
	UTitleCustomWidget* CustomInvenWidget = MainWidget->FindWidget<UTitleCustomWidget>(ETitleUIType::CustomeInven);

	if (nullptr == MainWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : MainWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == MenuWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : MenuWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == EntranceWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : EntranceWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == CustomInvenWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : CustomInvenWidget is null"), *FString(__FUNCSIG__));
		return;
	}

	ETitleUIType CurType = MainWidget->GetCurUIType();
	if (ETitleUIType::TitleCustom == CurType)
	{
		MenuWidget->MovePawnToHOME();
		return;
	}
	if (ETitleUIType::TitleEntrance == CurType)
	{
		int CurIndex = EntranceWidget->GetCurIndex() - 1;

		if (CurIndex < 0)
		{
			CurIndex = 0;
		}

		EntranceWidget->SetCurIndex(CurIndex);
		EntranceWidget->KeyInputButtonHover();
		return;
	}
	if (ETitleUIType::CustomeInven == CurType)
	{
		int CurIndex = CustomInvenWidget->GetCurIndex() - 1;

		if (CurIndex < 0)
		{
			CurIndex = 0;
		}

		CustomInvenWidget->SetCurIndex(CurIndex);
		CustomInvenWidget->SwitchToInvenMenu();
		return;
	}
}

void UUIInputManager::SwitchWidget_E()
{
	UTitleMainWidget* MainWidget = UFallGlobal::GetMainWidget(GetWorld());
	UTitleMenuWidget* MenuWidget = MainWidget->FindWidget<UTitleMenuWidget>(ETitleUIType::TitleMenu);
	UTitleEntranceWidget* EntranceWidget = MainWidget->FindWidget<UTitleEntranceWidget>(ETitleUIType::TitleEntrance);
	UTitleCustomWidget* CustomInvenWidget = MainWidget->FindWidget<UTitleCustomWidget>(ETitleUIType::CustomeInven);

	if (nullptr == MainWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : MainWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == MenuWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : MenuWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == EntranceWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : EntranceWidget is null"), *FString(__FUNCSIG__));
		return;
	}
	if (nullptr == CustomInvenWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : CustomInvenWidget is null"), *FString(__FUNCSIG__));
		return;
	}

	ETitleUIType CurType = MainWidget->GetCurUIType();
	if (ETitleUIType::TitleHome == CurType)
	{
		MenuWidget->MovePawnToCUSTOM();
		return;
	}
	if (ETitleUIType::TitleEntrance == CurType)
	{
		int CurIndex = EntranceWidget->GetCurIndex() + 1;

		if (CurIndex > EntranceWidget->GetButtonArray().Num() - 1)
		{
			CurIndex = EntranceWidget->GetButtonArray().Num() - 1;
		}

		EntranceWidget->SetCurIndex(CurIndex);
		EntranceWidget->KeyInputButtonHover();
		return;
	}
	if (ETitleUIType::CustomeInven == CurType)
	{
		int CurIndex = CustomInvenWidget->GetCurIndex() + 1;

		if (CurIndex > CustomInvenWidget->GetSwitcherNum() - 1)
		{
			CurIndex = CustomInvenWidget->GetSwitcherNum() - 1;
		}

		CustomInvenWidget->SetCurIndex(CurIndex);
		CustomInvenWidget->SwitchToInvenMenu();
		return;
	}
}

// Move
void UUIInputManager::SwitchWidget_W()
{
}

void UUIInputManager::SwitchWidget_A()
{
}

void UUIInputManager::SwitchWidget_S()
{
}

void UUIInputManager::SwitchWidget_D()
{
}
