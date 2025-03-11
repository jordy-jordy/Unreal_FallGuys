// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h" 

#include <Unreal_FallGuys.h>
#include <Global/FallConst.h>


APlayGameMode::APlayGameMode()
{
	ConnectedPlayers = 0;
}

void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 서버장에게만 보이는 메세지
	if (HasAuthority())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("서버: PlayGameMode가 시작되었습니다. 당신은 서버장입니다."));
	}
}

void APlayGameMode::ServerTravelToNextMap(const FString& url)
{
	//클라이언트 데리고 다같이 서버 트래블
	GetWorld()->ServerTravel(url,false);
}

void APlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	
	if (true == IsEndGame) return;
	if (CurFinishPlayer >= FinishPlayer)
	{
		IsEndGame = true;
		
		ServerTravelToNextMap(NextLevel);
		//
	}
}

// 플레이어 접속시 실행되는 함수
void APlayGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// 서버에서만 실행
	if (HasAuthority())
	{
		ConnectedPlayers++;
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("%s 에 접속합니다."), *CurrentLevelName);
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("서버: 플레이어가 접속했습니다. 현재 플레이어 수 = %d"), ConnectedPlayers);
		// 네트워크 동기화를 강제 실행하여 클라이언트와 데이터 맞추기
		ForceNetUpdate();
	}

	// 최소 인원 충족했을 경우
	if (IsMinPlayersReached())
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("플레이를 위한 최소 인원이 충족되었습니다. 게임 시작이 가능합니다."));
		StartGame();
	}
}

void APlayGameMode::OnRep_ConnectedPlayers()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("클라이언트: 플레이어 수 동기화 = %d"), ConnectedPlayers);
}

// 최소 인원 체크
bool APlayGameMode::IsMinPlayersReached()
{
	return ConnectedPlayers >= UFallConst::MinPlayerCount;
}

// 게임 시작
void APlayGameMode::StartGame_Implementation()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("게임이 시작되었습니다."));
	UFallConst::CanStart = true;
}

// 동기화 변수
void APlayGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameMode, ConnectedPlayers);
}


