// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/UIInputManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Global/FallGlobal.h"
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

	ATitlePlayerController* PlayerController = Cast<ATitlePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));;

	const UInputAction* Action = PlayerController->GetInputAction(TEXT("IA_UIMenuInput"));

	EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, this, &UUIInputManager::SwitchWidget);
}

void UUIInputManager::SwitchWidget()
{
	UTitleMainWidget* Widget = Cast<UTitleMainWidget>(UFallGlobal::GetMainWidget(GetWorld()));
	Widget->SwitchWidget(EUIType::TitleEntrance);
}


