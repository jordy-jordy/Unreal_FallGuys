// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Global/GlobalEnum.h"
#include "PlayUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetUserWidget(class UPlayUserWidget* _UserWidget)
	{
		UserWidget = _UserWidget;
	}

	UFUNCTION(BlueprintCallable)
	void WidgetInit(FName _PanelName);

	UFUNCTION(BlueprintCallable)
	void CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible);

protected:
	class UPlayUserWidget* CurUserWidget;
	EUIType CurUIType;

private:
	UPROPERTY(Category = "UI", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPlayUserWidget* UserWidget;

	UCanvasPanel* CanvasPanel;

	EUIType UIType;

	TMultiMap<EUIType, UPlayUserWidget*> Widgets;
};
