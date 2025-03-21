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
// ÄÚ½ºÆ¬ »ö»ó
//UENUM(BlueprintType)
//enum class ECostumeColor : uint8
//{
//	NONE UMETA(DisplayName = "³í"),
//	PINK UMETA(DisplayName = "ÇÎÅ©"),
//	YELLOW UMETA(DisplayName = "¿»·Î¿ì"),
//	BLUE UMETA(DisplayName = "ºí·ç"),
//	MAX UMETA(DisplayName = "¸Æ½º")
//};

UCLASS()
class UNREAL_FALLGUYS_API UCustomColorButtonWidget : public UCustomButtonWidget
{
	GENERATED_BODY()

public:

	//void Tick(FGeometry MyGeometry, float InDeltaTime) override;

	void NativeConstruct() override;

	FString GetCustomValueAsString(ECostumeColor _Color);


	

	UFUNCTION(BlueprintCallable)
	void ChangePawnColor();

	UFUNCTION(BlueprintCallable)
	void SetButtonColor();

	UFUNCTION()
	void GetResourceFromName();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	ECostumeColor Color = ECostumeColor::YELLOW;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	FString CustomName = TEXT("");

	UMaterial* Mat = nullptr;

	bool IsSeleted = false;

};
