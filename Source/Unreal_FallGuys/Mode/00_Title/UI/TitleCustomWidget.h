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
	void NativeConstruct();

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

	UFUNCTION(BlueprintImplementableEvent)
	void SelectCustom();

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
	void SetCurColorIndex(int _Index)
	{
		CurColorIndex = _Index;
	}

	UFUNCTION(BlueprintCallable)
	int GetCurColorIndex()
	{
		return CurColorIndex;
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

	UFUNCTION(BlueprintCallable)
	void SetCurCustomType(ECustomType _Type)
	{
		CurCustomType = _Type;
	}

	void ChangeType_Left();
	void ChangeType_Right();

	void ChangeColorIndex_Left();
	void ChangeColorIndex_Right();

	UFUNCTION(BlueprintImplementableEvent)
	void CustomColorMove();

	UFUNCTION(BlueprintImplementableEvent)
	void CustomColorReturn();

	UFUNCTION(BlueprintCallable)
	void CurColorSetting();

private:
	int CurIndex = 0;
	int AllSwitcherNum = 0;

	int CurColorIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TMap<ECustomType, class UObject*> CustomMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	ECustomType CurCustomType = ECustomType::MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UObject* CurColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrFiltered;
};
