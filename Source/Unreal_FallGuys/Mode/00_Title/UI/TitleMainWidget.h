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

	//UFUNCTION(BlueprintCallable)
	//void SwitchWidgetInMain(const FVector2D _Value);

	UFUNCTION(BlueprintCallable)
	void MainWidgetInit();

	UFUNCTION(BlueprintCallable)
	void CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible);

	void SetWidgetUIType(FString _CurWidgetName);

	UFUNCTION(BlueprintCallable)
	void SwitchWidget(EUIType _UIType);

	//void SwitchWidgetType(EUIType _UIType);

	TMultiMap<EUIType, UTitleUserWidget*> GetAllWidgets()
	{
		return Widgets;
	}

	UFUNCTION(BlueprintCallable)
	void AllWidgetHidden();

	UFUNCTION(BlueprintCallable)
	void AllWidgetSelfHitTestInvisible();

	UTitleUserWidget* GetCurWidget()
	{
		return CurWidget;
	}

	void SetCurWidget(UTitleUserWidget* _CurWidget)
	{
		CurWidget = _CurWidget;
	}

	UFUNCTION(BlueprintCallable)
	void WidgetHidden(EUIType _Type);

protected:

private:
	UCanvasPanel* CanvasPanel;
	EUIType UIType;
	EUIType CurUIType;
	UTitleUserWidget* CurWidget;
	TMultiMap<EUIType, UTitleUserWidget*> Widgets;
};
