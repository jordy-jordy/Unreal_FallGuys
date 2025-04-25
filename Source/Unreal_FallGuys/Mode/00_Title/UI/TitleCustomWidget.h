// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitleCustomWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleCustomWidget : public UTitleUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SwitchToCustomize();

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchToInvenMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void Move_UP();

	UFUNCTION(BlueprintImplementableEvent)
	void Move_DOWN();

	UFUNCTION(BlueprintImplementableEvent)
	void Move_LEFT();

	UFUNCTION(BlueprintImplementableEvent)
	void Move_RIGHT();

	UFUNCTION(BlueprintCallable)
	void SetCurIndex(int _Index)
	{
		CurIndex = _Index;
	}

	UFUNCTION(BlueprintCallable)
	int GetCurIndex()
	{
		return CurIndex;
	}

	UFUNCTION(BlueprintCallable)
	void SetCurChildIndex(int _Index)
	{
		CurChildIndex = _Index;
	}

	UFUNCTION(BlueprintCallable)
	int GetCurChildIndex()
	{
		return CurChildIndex;
	}

	UFUNCTION(BlueprintCallable)
	void SetSwitcherNum(int _Value)
	{
		AllSwitcherNum = _Value;
	}

	UFUNCTION(BlueprintCallable)
	int GetSwitcherNum()
	{
		return AllSwitcherNum;
	}

private:
	int CurIndex = 0;
	int AllSwitcherNum = 0;

	int CurChildIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrBottom;
};
