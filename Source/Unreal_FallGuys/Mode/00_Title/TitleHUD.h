// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TitleHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API ATitleHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	class UTitleMainWidget* GetMainWidget()
	{
		return MainWidget;
	}

protected:
	void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainWidgetClass = nullptr;

	class UTitleMainWidget* MainWidget = nullptr;
};
