// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitleMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleMenuWidget : public UTitleUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void ChangeButtonImage(UObject* _BaseResource, UObject* _SelectResource);

	UFUNCTION(BlueprintCallable)
	class UButton* GetCurButton()
	{
		return CurButton;
	}

	UFUNCTION(BlueprintCallable)
	void SetCurButton(int _Index)
	{
		CurButton = MenuButtons[_Index];
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UButton*> MenuButtons;

	class UButton* CurButton;
};
