// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/00_Title/UI/CustomButtonWidget.h"
#include <Global/GlobalEnum.h>
#include "CustomTopButtonWidget.generated.h"

/**
 *
 */
UCLASS()
class UNREAL_FALLGUYS_API UCustomTopButtonWidget : public UCustomButtonWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	ECostumeType Type = ECostumeType::TOP;


	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	FString TypeName = "";*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	FString ImgName = TEXT("");

	UTexture2D* NewTexture = nullptr;


	UFUNCTION()
	void SetButtonTopStyle();
	UFUNCTION(BlueprintCallable)
	void AttachCustomStaticMesh();
};
