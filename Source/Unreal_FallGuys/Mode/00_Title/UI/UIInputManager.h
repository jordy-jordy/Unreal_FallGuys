// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "UIInputManager.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UUIInputManager : public UTitleUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UIInput(const FVector2D _Value) override;

	UFUNCTION(BlueprintCallable)
	bool SwitchWidgetMenu(const FVector2D _Value);

	UFUNCTION(BlueprintCallable)
	bool SwitchWidgetHomeEnt(const FVector2D _Value);
	
};
