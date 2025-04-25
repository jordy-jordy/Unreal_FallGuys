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
};
