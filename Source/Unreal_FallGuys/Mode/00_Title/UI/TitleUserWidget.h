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
	void SetMenuWidget(class UTitleUserWidget* _UserWidget)
	{
		UserWidget = _UserWidget;
	}

	UFUNCTION(BlueprintCallable)
	void InputCheck(const FVector2D& _Value);

	UFUNCTION(BlueprintCallable)
	void WidgetInit();

	UFUNCTION(BlueprintCallable)
	void CreateChildWidget(TSubclassOf<UUserWidget> _Widget, bool _IsVisible);

	UFUNCTION(BlueprintCallable)
	void AllWidgetHidden();
private:
	UPROPERTY(Category = "UI", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTitleUserWidget* UserWidget;

	UCanvasPanel* CanvasPanel;

	UPROPERTY(Category = "UI", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UTitleUserWidget*> Widgets;
};
