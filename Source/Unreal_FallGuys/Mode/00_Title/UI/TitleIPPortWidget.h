// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/FallConst.h"
#include "Mode/00_Title/UI/TitleUserWidget.h"
#include "TitleIPPortWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UTitleIPPortWidget : public UTitleUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void SetIP(FString _IP)
	{
		IP = _IP;
		UFallConst::DefaultIP = _IP;
	}

	UFUNCTION(BlueprintCallable)
	FString& GetIP()
	{
		return UFallConst::DefaultIP;
	}

	UFUNCTION(BlueprintCallable)
	void SetPort(FString _Port)
	{
		Port = _Port;
		UFallConst::DefaultPort = _Port;
	}

	UFUNCTION(BlueprintCallable)
	FString& GetPort()
	{
		return UFallConst::DefaultPort;
	}
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString IP = UFallConst::DefaultIP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server", meta = (AllowPrivateAccess = "true"))
	FString Port = UFallConst::DefaultPort;
};
