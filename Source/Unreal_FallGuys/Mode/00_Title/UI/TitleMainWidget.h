// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitleMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleMainWidget : public UTitleUserWidget
{
	GENERATED_BODY()

public:
	UTitleMainWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SwitchWidgetInMain(const FVector2D _Value);

protected:

private:

};
