// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitleCustomizingMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleCustomizingMainWidget : public UTitleUserWidget
{
	GENERATED_BODY()

public:
	UTitleCustomizingMainWidget();
	~UTitleCustomizingMainWidget();
	
};
