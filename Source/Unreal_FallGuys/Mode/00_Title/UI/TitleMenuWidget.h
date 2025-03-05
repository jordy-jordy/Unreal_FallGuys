// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitleMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleMenuWidget : public UTitleUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	UFUNCTION(BlueprintCallable)
	void SwitchMenu(TSubclassOf<UUserWidget> _PrevWidget, TSubclassOf<UUserWidget> _NextWidget);

private:

};
