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
	
	// ���õ� ���� ��ȯ �Լ�
	UFUNCTION(BlueprintCallable)
	FString GetSelectedLevel() const;

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString IP = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString Port = TEXT("30000");

	void PopulateLevelSelection();

	// ���� ���� �޺��ڽ� (��� �޴�)
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* LevelSelection;

	// �޺��ڽ����� ������ ���� �����ϴ� �Լ�
	UFUNCTION()
	void OnLevelSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	// ���õ� ������ �����ϴ� ����
	FString SelectedLevel;

//LMH
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level", meta = (AllowPrivateAccess = "true"))
	FString MapName = TEXT("");
};
