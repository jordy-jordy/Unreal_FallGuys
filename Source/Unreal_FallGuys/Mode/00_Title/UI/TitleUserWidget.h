// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "TitleUserWidget.generated.h"

/**
 *
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
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
	UTitleUserWidget* GetCurUserWidget()
	{
		return CurUserWidget;
	}

	//UFUNCTION(BlueprintCallable)
	//void SetCurUserWidget(UTitleUserWidget* _Widget)
	//{
	//	//UTitleUserWidget* Widget = Cast<UTitleUserWidget>(_Widget);

	//	CurUserWidget = _Widget;


	TArray<UTitleUserWidget*> GetAllWidgets()
	{
		return Widgets;
	}

private:
	UPROPERTY(Category = "UI", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTitleUserWidget* UserWidget;
	class UTitleUserWidget* CurUserWidget;

	UCanvasPanel* CanvasPanel;

	UPROPERTY(Category = "UI", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UTitleUserWidget*> Widgets;
};
