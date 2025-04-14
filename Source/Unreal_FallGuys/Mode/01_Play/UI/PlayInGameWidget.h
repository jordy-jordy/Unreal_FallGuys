// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mode/01_Play/UI/PlayUserWidget.h"
// юс╫ц
#include "Mode/01_Play/PlayEnum.h"
#include "PlayInGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UPlayInGameWidget : public UPlayUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION(BlueprintCallable)
	EStagePhase CurStagePhase();

	UFUNCTION(BlueprintCallable)
	void ShowResult(bool _Value);

	UFUNCTION(BlueprintCallable)
	bool GetShowResult()
	{
		return IsShowResult;
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	bool IsShowResult = false;
};
