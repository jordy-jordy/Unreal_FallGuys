// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ChangeCurCostume(const FString& _CostumeName);
	
	virtual void NativeConstruct() override;
	
	// 선택된 레벨 반환 함수
	UFUNCTION(BlueprintCallable)
	FString GetSelectedLevel() const;

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString IP = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString Port = TEXT("30000");

	void PopulateLevelSelection();

	// 레벨 선택 콤보박스 (드롭 메뉴)
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* LevelSelection;

	// 콤보박스에서 선택한 값을 저장하는 함수
	UFUNCTION()
	void OnLevelSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	// 선택된 레벨을 저장하는 변수
	FString SelectedLevel;

//LMH
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level", meta = (AllowPrivateAccess = "true"))
	FString MapName = TEXT("");
};
