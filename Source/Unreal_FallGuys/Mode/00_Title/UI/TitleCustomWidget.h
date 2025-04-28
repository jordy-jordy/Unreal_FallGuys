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

#pragma region InputKeyFunction
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

	UFUNCTION(BlueprintImplementableEvent)
	void SelectCustom();
#pragma endregion

#pragma region CustomMenu
public:
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

	UFUNCTION(BlueprintCallable)
	void SetCurCustomType(ECustomType _Type)
	{
		CurCustomType = _Type;
	}

	ECustomType GetCurCustomType()
	{
		return CurCustomType;
	}

	void ChangeType_Left();
	void ChangeType_Right();

private:
	int CurIndex = 0;
	int AllSwitcherNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TMap<ECustomType, class UObject*> CustomMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	ECustomType CurCustomType = ECustomType::MAX;
#pragma endregion

#pragma region CustomColor
public:
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

	void ChangeColorIndex_Left();
	void ChangeColorIndex_Right();

	UFUNCTION(BlueprintCallable)
	void CurColorSetting();

	UFUNCTION(BlueprintImplementableEvent)
	void CustomColorMove();

	UFUNCTION(BlueprintImplementableEvent)
	void CustomColorReturn();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UObject* CurColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	int CurColorIndex = 0;
#pragma endregion

#pragma region CustomTop
public:
	void ChangeTopIndex_Up();
	void ChangeTopIndex_Down();
	void ChangeTopIndex_Left();
	void ChangeTopIndex_Right();

	UFUNCTION(BlueprintCallable)
	void CurTopSetting();

	UFUNCTION(BlueprintImplementableEvent)
	void CustomTopMove();

	UFUNCTION(BlueprintImplementableEvent)
	void CustomTopReturn();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UObject* CurTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	int CurTopIndex = 0;
#pragma endregion

#pragma region CustomBottom
public:
	void ChangeBottomIndex_Up();
	void ChangeBottomIndex_Down();
	void ChangeBottomIndex_Left();
	void ChangeBottomIndex_Right();

	UFUNCTION(BlueprintCallable)
	void CurBottomSetting();

	UFUNCTION(BlueprintImplementableEvent)
	void CustomBottomMove();

	UFUNCTION(BlueprintImplementableEvent)
	void CustomBottomReturn();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrBottom;

	// CustomBottom
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UObject* CurBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	int CurBottomIndex = 0;
#pragma endregion

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UObject*> ArrFiltered;
};
