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
// �ڽ�Ƭ ����
//UENUM(BlueprintType)
//enum class ECostumeColor : uint8
//{
//	NONE UMETA(DisplayName = "��"),
//	PINK UMETA(DisplayName = "��ũ"),
//	YELLOW UMETA(DisplayName = "���ο�"),
//	BLUE UMETA(DisplayName = "���"),
//	MAX UMETA(DisplayName = "�ƽ�")
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
