// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/TitleWidget.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"

#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/BaseGameInstance.h>


void UTitleWidget::PopulateLevelSelection()
{
    if (!LevelSelection) return;

    TArray<FString> LevelNames = UFallGlobal::GetAvailableLevels();

    for (const FString& LevelName : LevelNames)
    {
        LevelSelection->AddOption(LevelName);
    }

    if (LevelNames.Num() > 0)
    {
        LevelSelection->SetSelectedOption(LevelNames[0]);
        SelectedLevel = LevelNames[0];
    }
}

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PopulateLevelSelection();

    if (LevelSelection)
    {
        LevelSelection->OnSelectionChanged.AddDynamic(this, &UTitleWidget::OnLevelSelected);
    }
}

// ���õ� ������ ���� (OpenLevel ����)
void UTitleWidget::OnLevelSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (!SelectedItem.IsEmpty())
    {
        SelectedLevel = SelectedItem;
    }
}

// ���õ� ���� ��ȯ
FString UTitleWidget::GetSelectedLevel() const
{
    return SelectedLevel;
}