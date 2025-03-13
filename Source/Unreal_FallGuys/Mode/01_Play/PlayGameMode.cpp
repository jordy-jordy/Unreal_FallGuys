// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/01_Play/PlayGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h" 

#include <Unreal_FallGuys.h>
#include <Global/FallConst.h>
#include <Global/BaseGameInstance.h>


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
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (HasAuthority() && GameInstance)
	{
		GameInstance->InsSavePlayerInfo();
	}

	//클라이언트 데리고 다같이 서버 트래블
	GetWorld()->ServerTravel(url, false);
}

void APlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 게임 종료 체크
	if (true == IsEndGame) return;
	// 골인한 플레이어 수와 목표 인원 수 체크
	if (CurFinishPlayer >= FinishPlayer)
	{
		IsEndGame = true;

		ServerTravelToNextMap(NextLevel);
	}
}

// 플레이어 접속시 실행되는 함수
void APlayGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());

	// 서버에서만 실행
	if (HasAuthority())
	{
		ConnectedPlayers++;
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("%s 에 접속합니다."), *CurrentLevelName);
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("서버: 플레이어가 접속했습니다. 현재 플레이어 수 = %d"), ConnectedPlayers);

		if (false == GameInstance->IsMovedLevel)
		{
			AssignPlayerInfo(NewPlayer);
		}

		if (true == GameInstance->IsMovedLevel)
		{
			GameInstance->InsLoadPlayerInfo();

			// 모든 플레이어 정보 클라이언트에 동기화
			for (const auto& Entry : GameInstance->InsGetAllPlayerInfo())
			{
				APlayerController* PlayerController = Entry.Key;
				const FPlayerInfo& PlayerInfo = Entry.Value;

				if (PlayerController)
				{
					S2M_AssignPlayerInfo(PlayerController, PlayerInfo.Tag, PlayerInfo.Status);
				}
			}
		}

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

// 플레이어 정보 설정
void APlayGameMode::AssignPlayerInfo(APlayerController* _NewPlayer)
{
	if (!_NewPlayer)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("AssignPlayerInfo: NewPlayer is nullptr!"));
		return;
	}

	if (HasAuthority())
	{
		UBaseGameInstance* Ins = Cast<UBaseGameInstance>(GetGameInstance());

		if (Ins->InsGetAllPlayerInfo().Contains(_NewPlayer)) // 중복 체크
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("AssignPlayerInfo: Player already exists!"));
			return;
		}

		FString UniqueTag = FString::Printf(TEXT("Player%d"), PlayerCount);
		Ins->InsSetPlayerInfo(_NewPlayer, UniqueTag, EPlayerStatus::DEFAULT);
		_NewPlayer->Tags.AddUnique(FName(*UniqueTag));

		PlayerCount++;
		ForceNetUpdate();

		UE_LOG(FALL_DEV_LOG, Log, TEXT("서버: Player %s assigned tag: %s with status DEFAULT"), *_NewPlayer->GetName(), *UniqueTag);

		// 모든 클라이언트에게 정보 동기화
		if (_NewPlayer)
		{
			S2M_AssignPlayerInfo(_NewPlayer, UniqueTag, EPlayerStatus::DEFAULT);
		}
	}
}

// 플레이어 정보 동기화
void APlayGameMode::S2M_AssignPlayerInfo_Implementation(APlayerController* _NewPlayer, const FString& _Tag, EPlayerStatus _Status)
{
	UBaseGameInstance* Ins = Cast<UBaseGameInstance>(GetGameInstance());

	if (_NewPlayer)
	{
		if (!Ins->InsGetAllPlayerInfo().Contains(_NewPlayer)) // 클라이언트에서 없으면 추가
		{
			Ins->InsSetPlayerInfo(_NewPlayer, _Tag, _Status);
		}

		if (!_NewPlayer->Tags.Contains(FName(*_Tag))) // 중복 추가 방지
		{
			_NewPlayer->Tags.AddUnique(FName(*_Tag));
			UE_LOG(FALL_DEV_LOG, Log, TEXT("클라이언트: Player %s assigned tag: %s"), *_NewPlayer->GetName(), *_Tag);
		}
	}
}

void APlayGameMode::OnRep_ConnectedPlayers()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("클라이언트: ConnectedPlayers 동기화 = %d"), ConnectedPlayers);
}

void APlayGameMode::OnRep_PlayerCount()
{
	UE_LOG(FALL_DEV_LOG, Log, TEXT("클라이언트: PlayerCount 동기화 = %d"), PlayerCount);
}

// 동기화 변수
void APlayGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayGameMode, ConnectedPlayers);
	DOREPLIFETIME(APlayGameMode, PlayerCount);
}
