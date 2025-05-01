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
	virtual void Tick(float DeltaSeconds) override;

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
	UPROPERTY(EditAnywhere, Category = "INPUT", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_NextSpectate;
	UPROPERTY(EditAnywhere, Category = "INPUT", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_PrevSpectate;
	UPROPERTY(EditAnywhere, Category = "INPUT", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_ToggleTimer;

	void OnPrintPlayerInfo();         // [ : 플레이어 인포 출력 (PlayerState)
	void OnPrintConnectedPlayers();   // ] : 접속자 수 출력
	void OnPrintLevelName();          // - : 현재 레벨의 이름 출력
	void OnPrintLevelCinematicEnd();  // = : 레벨 시네마틱 끝나게
	void OnPrintCurFinishPlayer();	  // \ : 골인한 인원 및 목표 골인 인원 출력
	void OnNextSpectate();			  // Q
	void OnPrevSpectate();			  // E
	void OnToggleTimerAction();		  // Z

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

	// 이현정 : 서버에 컨트롤러 준비 완료 알림
	void CallReady();

	// 이현정 : 서버에 컨트롤러 준비 완료 알림
	UFUNCTION(Server, Reliable)
	void Server_NotifyControllerReadyForGame();
	void Server_NotifyControllerReadyForGame_Implementation();

	// 클라이언트 전용 View Target 설정 함수 : 일반 스테이지
	UFUNCTION(Client, Reliable)
	void Client_SetViewTargetByTag(FName _TargetTag);
	void Client_SetViewTargetByTag_Implementation(FName _TargetTag);

	UFUNCTION(Server, Reliable)
	void Server_NotifySettedRandomTarget(bool _Value);
	void Server_NotifySettedRandomTarget_Implementation(bool _Value);

	// 클라이언트 전용 View Target 설정 함수 : 중간 결과 화면
	UFUNCTION(Client, Reliable)
	void ClientWhoHidden_SetViewTargetByTag(FName _TargetTag);
	void ClientWhoHidden_SetViewTargetByTag_Implementation(FName _TargetTag);

	UFUNCTION(Server, Reliable)
	void Server_NotifySettedTarget(bool _Value);
	void Server_NotifySettedTarget_Implementation(bool _Value);

	// 레벨 이동 후 결과 화면에서 저장된 SpectateTargetTag를 사용해 카메라 세팅
	void Client_SetFailPlayerResultView(FName _Tag);

	// 레벨 이동 후 일반 스테이지에서 저장된 SpectateTargetTag를 사용해 카메라 세팅
	void Client_SetFailPlayerStageView(FName _Tag);

	// 결과 화면에서
	bool SettedTarget = false;
	bool SettedTarget_server = false; // 서버에서 클라 상태를 알기 위한 변수

	// 일반 스테이지에서
	bool SettedRandomTarget = false;
	bool SettedRandomTarget_server = false; // 서버에서 클라 상태를 알기 위한 변수

	UFUNCTION(Server, Reliable)
	void Server_NotifyHasSpectateTargetTag(APlayPlayerState* _PState, FName _Tag);
	void Server_NotifyHasSpectateTargetTag_Implementation(APlayPlayerState* _PState, FName _Tag);

	// 디버그용 : 팀전 제한시간 관련
	// 서버에게 타이머 정지/재시작 요청
	UFUNCTION(Server, Reliable)
	void Server_ToggleTimerPause();
	void Server_ToggleTimerPause_Implementation();


};
