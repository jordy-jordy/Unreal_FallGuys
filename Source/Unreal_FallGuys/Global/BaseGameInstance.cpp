// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include <Net/UnrealNetwork.h>
#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/FallConst.h>


UBaseGameInstance::UBaseGameInstance()
{
	// 보통 이 로그가 존재하는 위치를 알고 
	UE_LOG(FALL_DEV_LOG, Log, TEXT("%S(%u)> DataTableLoading Start"), __FUNCTION__, __LINE__);

	{
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BP/Global/Data/DT_GlobalDataTable.DT_GlobalDataTable'");
		ConstructorHelpers::FObjectFinder<UDataTable> FinderDataTables(*DataPath);
		if (true == FinderDataTables.Succeeded())
		{
			DataTables = FinderDataTables.Object;
		}

		//if (nullptr != DataTables)
		//{
		//	ActorDataTable = DataTables->FindRow<FDataTableRow>("DT_GlobalActorDataTable", nullptr)->Resources;
		//	if (nullptr == ActorDataTable)
		//	{
		//		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == ActorDataTable)"), __FUNCTION__, __LINE__);
		//	}
		//}

		if (nullptr != DataTables)
		{
			CostumeDataTable = DataTables->FindRow<FDataTableRow>("DT_CostumeDataTable", nullptr)->Resources;
			if (nullptr == CostumeDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == CostumeDataTable)"), __FUNCTION__, __LINE__);
			}
		}
	}

	// 흰색
	UE_LOG(FALL_DEV_LOG, Log, TEXT("%S(%u)> DataTableLoading End"), __FUNCTION__, __LINE__);
}

// 서버 오픈
void UBaseGameInstance::CServerStart(UWorld* _World, FString _Port)
{
    FString OpenLevel;
    FString LevelPath = TEXT("");

    UFallGlobal::AssetPackagePath(UWorld::StaticClass(), UFallConst::PlayLevelName, LevelPath);
    OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);

    UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
}

// 서버 접속
void UBaseGameInstance::CServerConnect(UWorld* _World, FString _IP, FString _Port)
{
    FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *_IP, *_Port);

    UGameplayStatics::OpenLevel(_World, FName(*ConnectLevelName));
}

void UBaseGameInstance::SaveSelectedCostume_Implementation(const FString& CostumeName)
{
	SelectedCostumeName = CostumeName;
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("Selected Costume Saved: %s"), *SelectedCostumeName);
}

FString UBaseGameInstance::GetSelectedCostume() const
{
	return SelectedCostumeName;
}

void UBaseGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseGameInstance, SelectedCostumeName); 
}