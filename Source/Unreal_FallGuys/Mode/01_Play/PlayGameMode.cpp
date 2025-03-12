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

	// 서버에서만 실행
	if (HasAuthority())
	{
		ConnectedPlayers++;
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("%s 에 접속합니다."), *CurrentLevelName);
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("서버: 플레이어가 접속했습니다. 현재 플레이어 수 = %d"), ConnectedPlayers);

		// 일정 시간 후 태그 할당 (비동기 대기)
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this, NewPlayer]()
			{
				AssignPlayerTag(NewPlayer);
			}, 1.0f, false);

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

// 플레이어 태그 설정
void APlayGameMode::AssignPlayerTag(APlayerController* _NewPlayer)
{
	if (!_NewPlayer)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("AssignPlayerTag: NewPlayer is nullptr!"));
		return;
	}

	// 서버에서만 태그 증가
	if (HasAuthority())
	{
		if (PlayerTags.Contains(_NewPlayer)) // 중복 체크
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("AssignPlayerTag: Player already has a tag!"));
			return;
		}

		FString UniqueTag = FString::Printf(TEXT("Player%d"), PlayerCount);
		PlayerTags.Add(_NewPlayer, UniqueTag);
		_NewPlayer->Tags.AddUnique(FName(*UniqueTag));

		// PlayerCount 증가 후 동기화
		PlayerCount++;
		ForceNetUpdate();

		UE_LOG(FALL_DEV_LOG, Log, TEXT("서버: Player %s assigned tag: %s"), *_NewPlayer->GetName(), *UniqueTag);

		// 모든 클라이언트에게 태그 동기화
		MulticastAssignPlayerTag(_NewPlayer, UniqueTag);
	}
}

// 플레이어 태그 동기화
void APlayGameMode::MulticastAssignPlayerTag_Implementation(APlayerController* _NewPlayer, const FString& _Tag)
{
	if (_NewPlayer)
	{
		if (!PlayerTags.Contains(_NewPlayer)) // 클라이언트에서 비어있을 경우 덮어쓰기
		{
			PlayerTags.Add(_NewPlayer, _Tag);
		}

		if (!_NewPlayer->Tags.Contains(FName(*_Tag))) // 중복 추가 방지
		{
			_NewPlayer->Tags.AddUnique(FName(*_Tag));
			UE_LOG(FALL_DEV_LOG, Log, TEXT("클라이언트: Player %s assigned tag: %s"), *_NewPlayer->GetName(), *_Tag);
		}
	}
}

// 특정 플레이어의 태그 반환
FString APlayGameMode::GetPlayerTag(APlayerController* _PlayerController) const
{
	if (!_PlayerController)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("GetPlayerTag: PlayerController is nullptr!"));
		return TEXT("Invalid");
	}

	if (PlayerTags.Contains(_PlayerController))
	{
		return PlayerTags[_PlayerController];
	}

	UE_LOG(FALL_DEV_LOG, Warning, TEXT("GetPlayerTag: PlayerController has no tag assigned!"));
	return TEXT("NoTag");
}

// 전체 플레이어 태그 리스트 반환
TMap<APlayerController*, FString> APlayGameMode::GetAllPlayerTags() const
{
	return PlayerTags;
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
