// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"

#include <Global/BaseGameInstance.h>

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

	// 이현정 : 클라이언트의 정보 세팅 및 동기화
public:
	UFUNCTION(Server, Reliable)
	void Server_SetClientPlayerInfo(const FString& _NickName, const FString& _Top, const FString& _Bot, const FString& _Color);
	void Server_SetClientPlayerInfo_Implementation(const FString& _NickName, const FString& _Top, const FString& _Bot, const FString& _Color);

	// 서버 → 클라이언트 : 승자 정보 전달용
	UFUNCTION(NetMulticast, Reliable)
	void Client_ReceiveWinnerInfo(const FWinnerInfo& _Info);
	void Client_ReceiveWinnerInfo_Implementation(const FWinnerInfo& _Info);

// 이현정 : EndLevel로 이동
public:
	UFUNCTION(Client, Reliable)
	void Client_TravelToEndLevel();
	void Client_TravelToEndLevel_Implementation();

	// 팀전 : 타이틀로 돌아가
	UFUNCTION(Client, Reliable)
	void Client_TravelToTitleLevel();
	void Client_TravelToTitleLevel_Implementation();

// 이현정 : 레벨 이동
public:
	// 다음 레벨로 이동하게 해주세요 (서버에게 요청)
	UFUNCTION(Server, Reliable)
	void Server_RequestSetCanMoveLevel(bool _b);
	void Server_RequestSetCanMoveLevel_Implementation(bool _b);

	// 이현정 : 서버에 준비 완료 알림
	void CallReady();

	// 이현정 : 서버에 준비 완료 알림
	UFUNCTION(Server, Reliable)
	void Server_NotifyReadyForTravel();
	void Server_NotifyReadyForTravel_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_CallReadyAfterTravel();
	void Client_CallReadyAfterTravel_Implementation();


};
