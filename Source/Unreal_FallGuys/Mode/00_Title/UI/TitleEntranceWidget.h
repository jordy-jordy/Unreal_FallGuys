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
	void WIdgetSelectInputAction(const FVector2D& _Value);

	UFUNCTION(BlueprintCallable)
	void WIdgetMoveInputAction(const FVector2D& _Value);

	UFUNCTION(BlueprintCallable)
	void PlayButtonAnim(int _CurIndex);

	UFUNCTION(BlueprintCallable)
	void SetInputState(bool _Value)
	{
		IsInput = _Value;
	}

	UFUNCTION(BlueprintCallable)
	int GetCurIndex()
	{
		return CurIndex;
	}

	class UButton* GetCurButton()
	{
		return CurButton;
	}

	UFUNCTION(BlueprintCallable)
	void ChangeButtonImage(FString _ButtonState);

	void ChangeButtonImage_Base();
	void ChangeButtonImage_Select();

private:
	EStageType TagetStage;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* StartServerButton;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* CompeteButton;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = "UI")
	UWidgetAnimation* ConnectButton;

	FWidgetAnimationDynamicEvent EntAnimStarted;
	FWidgetAnimationDynamicEvent EntAnimFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UButton*> ArrButtons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UButton* CurButton;

	int CurIndex = 0;

	bool IsInput = false;
};
