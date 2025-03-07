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
	void InputCheck(const FVector2D& _Value);

	UFUNCTION(BlueprintCallable)
	void WidgetInit(FName _PanelName);

	UFUNCTION(BlueprintCallable)
	void CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible);

	UFUNCTION(BlueprintCallable)
	void AllWidgetHidden();

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

	TMap<EUIType, UTitleUserWidget*> GetAllWidgets()
	{
		return Widgets;
	}

	EUIType GetCurUIType()
	{
		return CurUIType;
	}

	UFUNCTION(BlueprintCallable)
	void SwitchWidget(EUIType _UIType);

protected:

private:
	UPROPERTY(Category = "UI", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTitleUserWidget* UserWidget;
	class UTitleUserWidget* CurUserWidget;

	UCanvasPanel* CanvasPanel;

	EUIType UIType;
	EUIType CurUIType;

	TMap<EUIType, UTitleUserWidget*> Widgets;
};
