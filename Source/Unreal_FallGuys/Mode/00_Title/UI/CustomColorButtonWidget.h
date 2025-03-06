// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include <Components/Button.h>
#include "CustomColorButtonWidget.generated.h"


/**
 * 
 */
UENUM(BlueprintType)
enum class EColor : uint8
{
	TEST00,
	TEST01,
	TEST02,
};

UCLASS()
class UNREAL_FALLGUYS_API UCustomColorButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ButtonColor();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	EColor Color = EColor::TEST00;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* Button;
	
};
