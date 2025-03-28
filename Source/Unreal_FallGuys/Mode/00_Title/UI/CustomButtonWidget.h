// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Global/GlobalEnum.h>
#include "CustomButtonWidget.generated.h"


/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UCustomButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:

	void SetButtonStyle(UTexture2D* ImgTexture);
	void SetButtonStyle(UMaterialInterface* ImgMat);
	UFUNCTION()
	void AttachCustomStaticMesh();
	UFUNCTION()
	void DettachCustomStaticMesh();

	UFUNCTION(BlueprintCallable)
	void CustomStaticMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* Img_View;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	FString ImgName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	ECostumeType Type = ECostumeType::TOP;


};
