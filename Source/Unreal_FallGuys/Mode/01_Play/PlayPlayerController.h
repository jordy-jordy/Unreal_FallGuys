// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "PlayPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API APlayPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void AddMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetupInputComponentEvent();

protected:
	void BeginPlay() override;
	void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext = nullptr;

// 이현정 : 디버그용 함수 및 변수
private:
	UPROPERTY(EditAnywhere, Category = "DEBUG", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_PlayerInfo;

	UPROPERTY(EditAnywhere, Category = "DEBUG", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_ConnectedPlayers;

	UPROPERTY(EditAnywhere, Category = "DEBUG", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_LevelName;

	UPROPERTY(EditAnywhere, Category = "DEBUG", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_LevelCinematicEnd;

	UPROPERTY(EditAnywhere, Category = "DEBUG", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_CurFinishPlayer;

	void OnPrintPlayerInfo();         // [ : 플레이어 인포 출력 (PlayerState)
	void OnPrintConnectedPlayers();   // ] : 접속자 수 출력
	void OnPrintLevelName();          // - : 현재 레벨의 이름 출력
	void OnPrintLevelCinematicEnd();  // = : 레벨 시네마틱 끝나게
	void OnPrintCurFinishPlayer();	  // \ : 골인한 인원 및 목표 골인 인원 출력
};
