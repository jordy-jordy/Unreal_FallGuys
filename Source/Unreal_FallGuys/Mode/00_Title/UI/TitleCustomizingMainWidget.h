// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitleCustomizingMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleCustomizingMainWidget : public UTitleUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchToHome();

	UFUNCTION(BlueprintImplementableEvent)
	void Move_LEFT();

	UFUNCTION(BlueprintImplementableEvent)
	void Move_RIGHT();

	UFUNCTION(BlueprintImplementableEvent)
	void SelectInven();

	void ChangeType_Left();
	void ChangeType_Right();

	UFUNCTION(BlueprintCallable)
	void AddAllCustomInvens();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TMap<ECustomInvenType, UTitleUserWidget*> ArrCustomInvens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	ECustomInvenType CurInvenType = ECustomInvenType::MAX;

	int CurInvenIndex = 0;
};
