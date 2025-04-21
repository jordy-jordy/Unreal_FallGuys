// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/FallConst.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitlePlayerCountWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitlePlayerCountWidget : public UTitleUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void SetPlayerCount(int _PlayerCount)
	{
		PlayerCount = _PlayerCount;
		UFallConst::MinPlayerCount = _PlayerCount;
	}

	UFUNCTION(BlueprintCallable)
	int GetPlayerCount()
	{
		return UFallConst::MinPlayerCount;
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	int PlayerCount = UFallConst::MinPlayerCount;
};
