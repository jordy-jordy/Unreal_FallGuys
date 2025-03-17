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

	UTexture2D* NewTexture = nullptr;


	UFUNCTION()
	void SetButtonTopStyle();
	
};
