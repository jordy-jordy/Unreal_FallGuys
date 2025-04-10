// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"
#include "PlayMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayMainWidget : public UPlayUserWidget
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
	void SwitchWidget(EPlayUIType _UIType);

	TMultiMap<EPlayUIType, UPlayUserWidget*> GetAllWidgets()
	{
		return Widgets;
	}

	UFUNCTION(BlueprintCallable)
	void AllWidgetHidden();

	UFUNCTION(BlueprintCallable)
	void AllWidgetSelfHitTestInvisible();

	UPlayUserWidget* GetCurWidget()
	{
		return CurWidget;
	}

	void SetCurWidget(UPlayUserWidget* _CurWidget)
	{
		CurWidget = _CurWidget;
	}

	EPlayUIType& GetCurUIType()
	{
		return CurUIType;
	}

	UFUNCTION(BlueprintCallable)
	void TargetWidgetHidden(EPlayUIType _Type);

	UFUNCTION(BlueprintCallable)
	UPlayUserWidget* FindWidget(EPlayUIType _Type, int _Index = 0);

	UFUNCTION(BlueprintCallable)
	bool IsFailPlayer();

	UFUNCTION(BlueprintCallable)
	bool IsSuccessPlayer();
	
protected:

private:
	UCanvasPanel* CanvasPanel;
	EPlayUIType UIType;
	EPlayUIType CurUIType;
	UPlayUserWidget* CurWidget;
	TMultiMap<EPlayUIType, UPlayUserWidget*> Widgets;
};
