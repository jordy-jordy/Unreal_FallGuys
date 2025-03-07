// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include <Components/Button.h>
#include <Global/GlobalEnum.h>
#include "CustomColorButtonWidget.generated.h"


/**
 *
 */
 //UENUM(BlueprintType)
 //enum class EColor : uint8
 //{
 //	TEST00,
 //	TEST01,
 //	TEST02,
 //};

UCLASS()
class UNREAL_FALLGUYS_API UCustomColorButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//void Tick(FGeometry MyGeometry, float InDeltaTime) override;

	void NativeConstruct() override;

	UFUNCTION()
	void SetButtonStyle(FString Path);

	UFUNCTION(BlueprintCallable)
	void ChangePawnColor(ECostumeColor color);

	UFUNCTION(BlueprintCallable)
	void SetButtonColor();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	ECostumeColor Color = ECostumeColor::YELLOW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* Img_color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	FString CustomName = "";

	UTexture2D* NewTexture = nullptr;

	bool IsSeleted = false;

};
