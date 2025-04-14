// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/FallGlobal.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

#include <Unreal_FallGuys.h>
#include <Global/BaseGameInstance.h>
#include <Global/Data/PlayLevelDataTable.h>
#include <Global/Data/TeamPlayLevelDataTable.h>
#include <Global/Data/CostumeColorDataTable.h>
#include <Mode/00_Title/TitleHUD.h>
#include <Mode/01_Play/PlayGameMode.h>
#include <Mode/01_Play/TeamPlayGameMode.h>
#include <Mode/01_Play/PlayGameState.h>
#include <Mode/01_Play/TeamPlayGameState.h>


void UFallGlobal::AssetPackagePath(UClass* _Class, const FString& _AssetName, FString& _Path)
{
	TArray<FAssetData> AllAsset = AssetsPath(_Class);

	FName CheckName = FName(*_AssetName);

	for (const FAssetData& AssetData : AllAsset)
	{
		if (AssetData.AssetName == CheckName)
		{
			_Path = AssetData.PackageName.ToString();
			return;
		}
	}
}

TArray<FAssetData> UFallGlobal::AssetsPath(UClass* _Class)
{
	TArray<FAssetData> MapList;
	if (false == FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
	{
		return MapList;
	}

	IAssetRegistry& AssetRegistry = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

	if (AssetRegistry.IsLoadingAssets())
	{
		return MapList;
	}

	if (AssetRegistry.GetAssetsByClass(_Class->GetClassPathName(), MapList))
	{
		return MapList;
	}
	return MapList;
}

// 서버 시작
void UFallGlobal::ServerStart(UObject* _Object, FString _Port)
{
	UWorld* CurWorld = _Object->GetWorld();
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Object->GetWorld()->GetGameInstance());
	GameIns->CServerStart(CurWorld, _Port);
}

// 서버 시작 : 선택 필요
void UFallGlobal::SelectedServerStart(UObject* _Object, FString _Port, FString _OpenLevel)
{
	UWorld* CurWorld = _Object->GetWorld();
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Object->GetWorld()->GetGameInstance());
	GameIns->InsSelectedServerStart(CurWorld, _Port, _OpenLevel);
}

// 서버 접속
void UFallGlobal::ServerConnect(UObject* _Object, FString _IP, FString _Port)
{
	UWorld* CurWorld = _Object->GetWorld();
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(_Object->GetWorld()->GetGameInstance());
	GameIns->CServerConnect(CurWorld, _IP, _Port);
}

// BaseGameInstance : 저장된 코스튬의 컬러 반환
FString UFallGlobal::GetCostumeColor(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeColor();
}

// BaseGameInstance : 저장된 코스튬의 상의 반환
FString UFallGlobal::GetCostumeTop(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeTop();
}

// BaseGameInstance : 저장된 코스튬의 하의 반환
FString UFallGlobal::GetCostumeBot(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeBot();
}

// BaseGameInstance : Pawn의 코스튬 컬러 변경
void UFallGlobal::ChangeCostumeColor(APawn* _Pawn, const FString& _CostumeColor)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostumeColor(_Pawn, _CostumeColor);
}

// BaseGameInstance : Pawn의 코스튬 상의 변경
void UFallGlobal::ChangeCostumeTop(APawn* _Pawn, UStaticMeshComponent* _UpComp, const FString& _CostumeTop)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostumeTop(_Pawn, _UpComp, _CostumeTop);
}

// BaseGameInstance : Pawn의 코스튬 하의 변경
void UFallGlobal::ChangeCostumeBot(APawn* _Pawn, UStaticMeshComponent* _LowComp, const FString& _CostumeBot)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostumeBot(_Pawn, _LowComp, _CostumeBot);
}

// BaseGameInstance : Pawn의 코스튬 컬러 변경 - 저장 안함
void UFallGlobal::ChangeCostumeColorWithOutSave(APawn* _Pawn, const FString& _CostumeColor)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostumeColorWithOutSave(_Pawn, _CostumeColor);
}

// BaseGameInstance : Pawn의 코스튬 상의 변경 - 저장 안함
void UFallGlobal::ChangeCostumeTopWithOutSave(APawn* _Pawn, UStaticMeshComponent* _UpComp, const FString& _CostumeTop)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostumeTopWithOutSave(_Pawn, _UpComp, _CostumeTop);
}

// BaseGameInstance : Pawn의 코스튬 하의 변경 - 저장 안함
void UFallGlobal::ChangeCostumeBotWithOutSave(APawn* _Pawn, UStaticMeshComponent* _LowComp, const FString& _CostumeBot)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	GameIns->InsChangeCostumeBotWithOutSave(_Pawn, _LowComp, _CostumeBot);
}

// BaseGameInstance : 저장된 코스튬 컬러의 스켈레탈 메시 반환
USkeletalMesh* UFallGlobal::GetCostumeColorMesh(APawn* _Pawn, const FString& _MeshName)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeColorMesh(_Pawn, _MeshName);
}

// BaseGameInstance : 코스튬 컬러 UI용 머티리얼 반환
UMaterialInterface* UFallGlobal::GetCostumeColorUIMaterial(APawn* _Pawn, const FString& _ColorName)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeColorUIMaterial(_ColorName);
}

// BaseGameInstance : 저장된 코스튬의 스테틱 메시 반환
UStaticMesh* UFallGlobal::GetCostumeMesh(APawn* _Pawn, const FString& _MeshName)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetCostumeMesh(_Pawn, _MeshName);
}

// BaseGameInstance : 리소스의 스테틱 메시 머티리얼 반환
UMaterialInterface* UFallGlobal::GetResourceMeshMaterial(const FString& _ColorName)
{
	UBaseGameInstance* GameIns = GWorld->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetResourceMeshMaterial(_ColorName);
}

// BaseGameInstance : 리소스의 스테틱 메시 반환
UStaticMesh* UFallGlobal::GetResourceMesh(const FString& _MeshName)
{
	UWorld* World = GWorld;
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(World->GetGameInstance());
	return GameIns->InsGetResourceMesh(World, _MeshName);
}

// BaseGameInstance : 닉네임 반환
FString UFallGlobal::GetNickname(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetNickname();
}

// BaseGameInstance : 닉네임 저장
void UFallGlobal::ChangeNickname(APawn* _Pawn, const FString& _NewNickname)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsChangeNickname(_NewNickname);
}

// BaseGameInstance : 랜덤 스테이지 반환
FString UFallGlobal::GetRandomLevel(APawn* _Pawn)
{
	UBaseGameInstance* GameIns = _Pawn->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetRandomLevel();
}

// BaseGameInstance : 랜덤 스테이지 반환 : Pawn없이
FString UFallGlobal::GetRandomLevelWithOutPawn()
{
	UBaseGameInstance* GameIns = GWorld->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetRandomLevel();
}

// BaseGameInstance : 랜덤 팀전 스테이지 반환 : Pawn없이
FString UFallGlobal::GetRandomTeamLevel()
{
	UBaseGameInstance* GameIns = GWorld->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetRandomTeamLevel();
}


/////////////////////////////////////////////////////////////////////////////////////////////////


// PlayGameState : 랜덤 레벨 함수에서 얻은 에셋 이름 반환
FString UFallGlobal::GetLevelAssetName()
{
	APlayGameState* FallState = Cast<APlayGameState>(GWorld->GetGameState());
	return FallState->GetLevelAssetName_STATE();
}

// PlayGameState : 랜덤 레벨 함수에서 얻은 이름 반환
FString UFallGlobal::GetLevelName()
{
	APlayGameState* FallState = Cast<APlayGameState>(GWorld->GetGameState());
	return FallState->GetLevelName_STATE();
}

// PlayGameState : 현재의 스테이지 타입(개인전, 팀전)을 얻음
EStageType UFallGlobal::GetCurStageType()
{
	APlayGameState* FallState = Cast<APlayGameState>(GWorld->GetGameState());
	return FallState->GetLevelType_STATE();
}

// PlayGameState : 현재 스테이지의 종료를 판단하는 기준 상태
EPlayerStatus UFallGlobal::GetStageEndCondition()
{
	APlayGameState* FallState = Cast<APlayGameState>(GWorld->GetGameState());
	return FallState->GetEndCondition_STATE();
}

// PlayGameState : 플레이 가이드 반환
FString UFallGlobal::GetPlayGuideFromAssetName(const FString& _AssetName)
{
	APlayGameState* FallState = Cast<APlayGameState>(GWorld->GetGameState());
	return FallState->GetPlayGuide_STATE();
}

// PlayGameState : 골 가이드 반환
FString UFallGlobal::GetGoalGuideFromAssetName(const FString& _AssetName)
{
	APlayGameState* FallState = Cast<APlayGameState>(GWorld->GetGameState());
	return FallState->GetGoalGuide_STATE();
}

// PlayGameState : 레벨 이미지 반환
UTexture2D* UFallGlobal::GetLevelImageFromAssetName(const FString& _AssetName)
{
	APlayGameState* FallState = Cast<APlayGameState>(GWorld->GetGameState());
	return FallState->GetLevelIMG_STATE();
}

// PlayGameState : 레벨 태그 이미지 반환
UTexture2D* UFallGlobal::GetLevelTagImage()
{
	APlayGameState* FallState = Cast<APlayGameState>(GWorld->GetGameState());
	return FallState->GetLevelTagIMG_STATE();
}

// PlayGameState : 레이싱이야 생존이야
FString UFallGlobal::GetStageGoalType()
{
	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	return FallState->GetSTATEStageGoalType();
}


/////////////////////////////////////////////////////////////////////////////////////////////////


// PlayGameState : 현재 접속한 플레이어 수 반환
int UFallGlobal::GetConnectedPlayers()
{
	APlayGameState* PlayState = Cast<APlayGameState>(GWorld->GetGameState());
	return PlayState->GetConnectedPlayers();
}

// PlayGameState : Count Down Time 반환 - 실시간으로 줄어드는 시간
float UFallGlobal::GetCountDownTime()
{
	APlayGameState* PlayState = Cast<APlayGameState>(GWorld->GetGameState());
	return PlayState->GetCountDownTime();
}

// PlayGameState : Is Count Down Over 반환 - 카운트 다운 끝났어?
bool UFallGlobal::GetIsCountDownOver()
{
	APlayGameState* PlayState = Cast<APlayGameState>(GWorld->GetGameState());
	return PlayState->GetIsCountDownOver();
}

// PlayGameState : 레벨 시네마틱 시작해도 돼?
bool UFallGlobal::GetCanStartLevelCinematic()
{
	APlayGameState* PlayState = GWorld->GetGameState<APlayGameState>();
	return PlayState->GetCanStartLevelCinematic();
}

// PlayGameState : 레벨 시네마틱 끝났어?
bool UFallGlobal::GetIsLevelCinematicEnd()
{
	APlayGameState* PlayState = GWorld->GetGameState<APlayGameState>();
	return PlayState->GetIsLevelCinematicEnd();
}

// BaseGameInstance : 플레이어 상태를 리셋하는 함수
void UFallGlobal::ResetPlayerCondition()
{
	UBaseGameInstance* GameIns = GWorld->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsResetPlayerCondition();
}

// BaseGameInstance : 닉네임 설정했니?
bool UFallGlobal::GetHasNickname()
{
	UBaseGameInstance* GameIns = GWorld->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetHasNickname();
}

// TeamPlayGameState : 레드팀 점수 반환
int32 UFallGlobal::GetREDTeamScore()
{
	ATeamPlayGameState* TeamState = GWorld->GetGameState<ATeamPlayGameState>();
	return TeamState->GetGameStateREDTeamScore();
}

// TeamPlayGameState : 블루팀 점수 반환
int32 UFallGlobal::GetBLUETeamScore()
{
	ATeamPlayGameState* TeamState = GWorld->GetGameState<ATeamPlayGameState>();
	return TeamState->GetGameStateBLUETeamScore();
}

// PlayGameState : 접속자 수를 1씩 감소
void UFallGlobal::MinusConnectedPlayers()
{
	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	FallState->MinusConnectedPlayers();
}

// PlayGameState : 결과 화면이니?
bool UFallGlobal::GetIsResultLevel()
{
	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	return FallState->GetGameStateIsResultLevel();
}

// PlayGameMode : 레벨 이동 해도 된다는 걸 알려주는 함수
void UFallGlobal::SetCanMoveLevel(bool _Value)
{
	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	FallState->S2C_SetCanMoveLevel(_Value);
}

// PlayGameState : 게임 시작했니?
bool UFallGlobal::GetGameStarted()
{
	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	return FallState->GetGameStateGameStarted();
}

// PlayGameState : 목표 인원 세팅 완료했니?
bool UFallGlobal::GetSettedGoalCountDone()
{
	APlayGameState* FallState = GWorld->GetGameState<APlayGameState>();
	return FallState->GetGameStateSettedGoalCount();
}

// TeamPlayGameState : 남은 시간이 뭐야
float UFallGlobal::GetRemainingTime()
{
	ATeamPlayGameState* FallTeamState = GWorld->GetGameState<ATeamPlayGameState>();
	return FallTeamState->GetRemainingTime();
}

// BaseGameInstance : 승리한 유저의 닉네임 가져오기 - EndLevel 전용
FString UFallGlobal::GetWinnerNickname()
{
	UBaseGameInstance* GameIns = GWorld->GetGameInstance<UBaseGameInstance>();
	return GameIns->InsGetWinnerNickname();
}

// 이재영 : 메인위젯을 얻는 함수
UTitleMainWidget* UFallGlobal::GetMainWidget(UWorld* _World)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(_World, 0);

	if (nullptr == PlayerController)
	{
		UE_LOG(FALL_DEV_LOG, Fatal, TEXT("%S(%u)> if (nullptr == PlayerController)"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	ATitleHUD* TitleHUD = Cast<ATitleHUD>(PlayerController->GetHUD());

	if (nullptr == TitleHUD)
	{
		UE_LOG(FALL_DEV_LOG, Fatal, TEXT("%S(%u)> if (nullptr == PlayerController)"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return TitleHUD->GetMainWidget();
}

