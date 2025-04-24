// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void SetTargetStageType(EStageType _TargetStage)
	{
		TagetStage = _TargetStage;
	}

	UFUNCTION(BlueprintCallable)
	EStageType GetTargetStageType()
	{
		return TagetStage;
	}

	UFUNCTION(BlueprintCallable)
	void PlayButtonAnim(int _CurIndex);

	UFUNCTION(BlueprintCallable)
	int GetCurIndex()
	{
		return CurIndex;
	}

	UFUNCTION(BlueprintCallable)
	class UButton* GetCurButton()
	{
		return CurButton;
	}

	UFUNCTION(BlueprintCallable)
	void SetCurButton(int _Index)
	{
		CurIndex = _Index;
		CurButton = ArrButtons[_Index];
	}

	UFUNCTION(BlueprintCallable)
	void ChangeButtonImage();

private:
	EStageType TagetStage;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* StartServerButton;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* CompeteButton;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* ConnectButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UButton*> ArrButtons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UButton* CurButton;

	int CurIndex = 0;
};
