// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "Components/CanvasPanel.h"
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
	void MenuWidgetInit();

	UFUNCTION(BlueprintCallable)
	void CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible);

	UFUNCTION(BlueprintCallable)
	void SwitchMenu(FString _PrevPanel, FString _NextPanel);

private:
	UCanvasPanel* MenuCanvasPanel;
	TArray<UTitleUserWidget*> Widgets;
};
