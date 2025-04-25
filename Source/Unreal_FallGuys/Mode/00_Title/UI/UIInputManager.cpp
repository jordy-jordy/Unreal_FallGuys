// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/UIInputManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Global/FallGlobal.h"
#include "Unreal_FallGuys.h"
#include "Mode/00_Title/TitlePlayerController.h"
#include "Mode/00_Title/UI/TitleMainWidget.h"
#include "Mode/00_Title/UI/TitleEntranceWidget.h"
#include "Mode/00_Title/UI/TitlePlayerCountWidget.h"
#include "Mode/00_Title/UI/TitleMenuWidget.h"


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

	const UInputAction* UI_Space = PlayerController->GetInputAction(TEXT("IA_UI_Space"));
	const UInputAction* UI_Esc = PlayerController->GetInputAction(TEXT("IA_UI_Esc"));
	const UInputAction* UI_Q = PlayerController->GetInputAction(TEXT("IA_UI_Q"));
	const UInputAction* UI_E = PlayerController->GetInputAction(TEXT("IA_UI_E"));

	EnhancedInputComponent->BindAction(UI_Space, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_SPACE);
	EnhancedInputComponent->BindAction(UI_Esc, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_ESC);
	EnhancedInputComponent->BindAction(UI_Q, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_Q);
	EnhancedInputComponent->BindAction(UI_E, ETriggerEvent::Completed, this, &UUIInputManager::SwitchWidget_E);
}

void UUIInputManager::SwitchWidget_SPACE()
{
	UTitleMainWidget* MainWidget = UFallGlobal::GetMainWidget(GetWorld());
	UTitleEntranceWidget* EntranceWidget = MainWidget->FindWidget<UTitleEntranceWidget>(ETitleUIType::TitleEntrance);

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

	ETitleUIType CurType = MainWidget->GetCurUIType();
	if (ETitleUIType::TitleHome == CurType)
	{
		MainWidget->SwitchWidget(ETitleUIType::TitleEntrance);
		return;
	}
	else if (ETitleUIType::TitleEntrance == CurType)
	{
		EntranceWidget->KeyInputButtonChoice();

		if (2 != EntranceWidget->GetCurIndex())
		{
			MainWidget->SwitchWidget(ETitleUIType::TitlePlayerCount);
			return;
		}
		else
		{
			MainWidget->SwitchWidget(ETitleUIType::TitleIPPort);
			return;
		}
	}
	else if (ETitleUIType::TitlePlayerCount == CurType)
	{
		return;
	}
}

void UUIInputManager::SwitchWidget_ESC()
{
	UTitleMainWidget* MainWidget = UFallGlobal::GetMainWidget(GetWorld());

	if (nullptr == MainWidget)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("[%s] : MainWidget is null"), *FString(__FUNCSIG__));
		return;
	}

	ETitleUIType CurType = MainWidget->GetCurUIType();
	if (ETitleUIType::TitleEntrance == CurType)
	{
		MainWidget->SwitchWidget(ETitleUIType::TitleHome);
		return;
	}
	else if (ETitleUIType::TitlePlayerCount == CurType)
	{
		MainWidget->SwitchWidget(ETitleUIType::TitleEntrance);
		return;
	}
	else if (ETitleUIType::TitleIPPort == CurType)
	{
		MainWidget->SwitchWidget(ETitleUIType::TitleEntrance);
		return;
	}
}

void UUIInputManager::SwitchWidget_Q()
{
	UTitleMainWidget* MainWidget = UFallGlobal::GetMainWidget(GetWorld());
	UTitleMenuWidget* MenuWidget = MainWidget->FindWidget<UTitleMenuWidget>(ETitleUIType::TitleMenu);

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

	ETitleUIType CurType = MainWidget->GetCurUIType();
	if (ETitleUIType::TitleCustom == CurType)
	{
		MenuWidget->MovePawnToHOME();
		MainWidget->SwitchWidget(ETitleUIType::TitleHome);
		return;
	}
}

void UUIInputManager::SwitchWidget_E()
{
	UTitleMainWidget* MainWidget = UFallGlobal::GetMainWidget(GetWorld());
	UTitleMenuWidget* MenuWidget = MainWidget->FindWidget<UTitleMenuWidget>(ETitleUIType::TitleMenu);

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

	ETitleUIType CurType = MainWidget->GetCurUIType();
	if (ETitleUIType::TitleHome == CurType)
	{
		MenuWidget->MovePawnToCUSTOM();
		MainWidget->SwitchWidget(ETitleUIType::TitleCustom);
		return;
	}
}
