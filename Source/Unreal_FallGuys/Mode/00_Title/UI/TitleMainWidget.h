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
	void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void MainWidgetInit();

	UFUNCTION(BlueprintCallable)
	void CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible);

	void SetWidgetUIType(FString _CurWidgetName);

	UFUNCTION(BlueprintCallable)
	void SwitchWidget(ETitleUIType _UIType);

	TMultiMap<ETitleUIType, UTitleUserWidget*> GetAllWidgets()
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
	ETitleUIType& GetCurUIType()
	{
		return CurUIType;
	}

	UFUNCTION(BlueprintCallable)
	void WidgetHidden(ETitleUIType _Type);

	UFUNCTION(BlueprintCallable)
	UTitleUserWidget* FindWidget(ETitleUIType _Type, int _Index = 0);

protected:

private:
	UCanvasPanel* CanvasPanel;
	ETitleUIType UIType;
	ETitleUIType CurUIType;
	UTitleUserWidget* CurWidget;
	TMultiMap<ETitleUIType, UTitleUserWidget*> Widgets;
};
