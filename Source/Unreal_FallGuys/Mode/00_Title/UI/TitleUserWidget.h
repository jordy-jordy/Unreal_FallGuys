// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Global/GlobalEnum.h"
#include "TitleUserWidget.generated.h"

/**
 *
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTitleUserWidget(const FObjectInitializer& ObjectInitializer);

	void SetUserWidget(class UTitleUserWidget* _UserWidget)
	{
		UserWidget = _UserWidget;
	}

	UFUNCTION(BlueprintCallable)
	bool InputCheck(const FVector2D& _Value);

	UFUNCTION(BlueprintCallable)
	void WidgetInit(FName _PanelName);

	UFUNCTION(BlueprintCallable)
	void CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible);

	UFUNCTION(BlueprintCallable)
	void AllWidgetHidden();

	UFUNCTION(BlueprintCallable)
	void AllWidgetSelfHitTestInvisible();

	UFUNCTION(BlueprintCallable)
	UTitleUserWidget* GetCurUserWidget()
	{
		return CurUserWidget;
	}

	UFUNCTION(BlueprintCallable)
	void SetCurUserWidget(UTitleUserWidget* _Widget)
	{
		CurUserWidget = _Widget;
	}

	TMultiMap<EUIType, UTitleUserWidget*> GetAllWidgets()
	{
		return Widgets;
	}

	UFUNCTION(BlueprintCallable)
	void SwitchWidget(EUIType _UIType);

	EUIType GetCurUIType()
	{
		return CurUIType;
	}

	void SetCurUIType(EUIType _UIType)
	{
		CurUIType = _UIType;
	}

protected:
	class UTitleUserWidget* CurUserWidget;
	EUIType CurUIType;

private:
	UPROPERTY(Category = "UI", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTitleUserWidget* UserWidget;

	UCanvasPanel* CanvasPanel;

	EUIType UIType;

	TMultiMap<EUIType, UTitleUserWidget*> Widgets;
};
