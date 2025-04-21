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

	void NativeConstruct();

	void SetMainWidget(class UTitleMainWidget* _MainWidget)
	{
		MainWidget = _MainWidget;
	}

	UFUNCTION(BlueprintCallable)
	UTitleMainWidget* GetMainWidget()
	{
		return MainWidget;
	}

	UFUNCTION(BlueprintCallable)
	bool InputCheck(const FVector2D _Value);

protected:

private:
	UPROPERTY(Category = "UI", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTitleMainWidget* MainWidget;
};
