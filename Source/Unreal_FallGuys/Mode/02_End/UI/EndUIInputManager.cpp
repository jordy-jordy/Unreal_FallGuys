// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/02_End/UI/EndUIInputManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/02_End/EndPlayerController.h"


// Sets default values for this component's properties
UEndUIInputManager::UEndUIInputManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEndUIInputManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEndUIInputManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEndUIInputManager::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(_PlayerInputComponent);

	AEndPlayerController* PlayerController = Cast<AEndPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//const UInputAction* UI_Esc = PlayerController->GetInputAction(TEXT("IA_UI_Esc"));

	//EnhancedInputComponent->BindAction(UI_Esc, ETriggerEvent::Completed, this, &UEndUIInputManager::SwitchWidget_ESC);
}

