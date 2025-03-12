// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/FallConst.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitleEntranceWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleEntranceWidget : public UTitleUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void VisibleInputPanel(UTitleUserWidget* _CurWidget);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString IP = UFallConst::DefaultIP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString Port = UFallConst::DefaultPort;
};
