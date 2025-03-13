// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/00_Title/UI/CustomButtonWidget.h"
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
class UNREAL_FALLGUYS_API UCustomColorButtonWidget : public UCustomButtonWidget
{
	GENERATED_BODY()

public:

	//void Tick(FGeometry MyGeometry, float InDeltaTime) override;

	void NativeConstruct() override;

	

	UFUNCTION(BlueprintCallable)
	void ChangePawnColor(ECostumeColor color);

	UFUNCTION(BlueprintCallable)
	void SetButtonColor();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	ECostumeColor Color = ECostumeColor::YELLOW;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	FString CustomName = "";

	UMaterial* Mat = nullptr;

	bool IsSeleted = false;

};
