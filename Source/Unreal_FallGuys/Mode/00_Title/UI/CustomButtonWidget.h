// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include "CustomButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UCustomButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	//UFUNCTION()
	void SetButtonStyle(UMaterial* Mat,FString Path);
	//UFUNCTION()
	void SetButtonStyle(UTexture2D* Texture,FString Path);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* Img_View;
};
