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
	void SetMainWidget(class UPlayMainWidget* _MainWidget)
	{
		MainWidget = _MainWidget;
	}

	UFUNCTION(BlueprintCallable)
	UPlayMainWidget* GetMainWidget()
	{
		return MainWidget;
	}

protected:

private:
	UPROPERTY(Category = "UI", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPlayMainWidget* MainWidget;
};
