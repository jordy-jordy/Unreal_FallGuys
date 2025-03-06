// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include <Net/UnrealNetwork.h>
#include <Unreal_FallGuys.h>
#include <Global/FallGlobal.h>
#include <Global/FallConst.h>


UBaseGameInstance::UBaseGameInstance()
{
	UE_LOG(FALL_DEV_LOG, Log, TEXT("%S(%u)> DataTableLoading Start"), __FUNCTION__, __LINE__);
	{
		FString DataPath = UFallConst::GlobalDataTablePath;
		ConstructorHelpers::FObjectFinder<UDataTable> FinderDataTables(*DataPath);
		if (true == FinderDataTables.Succeeded())
		{
			DataTables = FinderDataTables.Object;
		}

		if (nullptr != DataTables)
		{
			CostumeDataTable = DataTables->FindRow<FDataTableRow>("DT_CostumeDataTable", nullptr)->Resources;
			if (nullptr == CostumeDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == CostumeDataTable)"), __FUNCTION__, __LINE__);
			}
		}

		//if (nullptr != DataTables)
		//{
		//	ActorDataTable = DataTables->FindRow<FDataTableRow>("DT_GlobalActorDataTable", nullptr)->Resources;
		//	if (nullptr == ActorDataTable)
		//	{
		//		UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == ActorDataTable)"), __FUNCTION__, __LINE__);
		//	}
		//}
	}
	// 흰색
	UE_LOG(FALL_DEV_LOG, Log, TEXT("%S(%u)> DataTableLoading End"), __FUNCTION__, __LINE__);
}

// 서버 오픈
void UBaseGameInstance::CServerStart(UWorld* _World, FString _Port)
{
	if (!_World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerStart: _World is nullptr"));
		return;
	}

    FString OpenLevel;
    FString LevelPath = TEXT("");

    UFallGlobal::AssetPackagePath(UWorld::StaticClass(), UFallConst::PlayLevelName, LevelPath);
    OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);

    UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
}

// 서버 접속
void UBaseGameInstance::CServerConnect(UWorld* _World, FString _IP, FString _Port)
{
	if (!_World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerConnect: _World is nullptr"));
		return;
	}

    FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *_IP, *_Port);

    UGameplayStatics::OpenLevel(_World, FName(*ConnectLevelName));
}

void UBaseGameInstance::SaveSelectedCostume_Implementation(const FString& _CostumeName)
{
	SelectedCostumeName = _CostumeName;
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

void UBaseGameInstance::OnRep_SelectedCostumeName()
{
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("SelectedCostumeName Replicated: %s"), *SelectedCostumeName);
}

void UBaseGameInstance::ApplySavedCostume_Implementation(APawn* _Pawn)
{
	if (!_Pawn) return;

	UWorld* World = _Pawn->GetWorld();
	UBaseGameInstance* GameIns = Cast<UBaseGameInstance>(World->GetGameInstance());
	if (!GameIns) return;

	// 저장된 코스튬 가져오기
	FString SavedCostume = GameIns->GetSelectedCostume();
	if (SavedCostume.IsEmpty()) return;

	// 기존 ChangeCostume 함수 호출
	ChangeCostume(_Pawn, SavedCostume);
}

void UBaseGameInstance::ChangeCostume(APawn* _Pawn, const FString& _CostumeName)
{
	UWorld* World = _Pawn->GetWorld();
	const FCostumeDataRow* CostumeData = UGlobalDataTable::GetCostumeData(World, _CostumeName);
	if (CostumeData && CostumeData->CostumeMesh)
	{
		// _Pawn의 스켈레탈 메시 가져오기
		if (USkeletalMeshComponent* MeshComp = _Pawn->FindComponentByClass<USkeletalMeshComponent>())
		{
			MeshComp->SetSkeletalMesh(CostumeData->CostumeMesh);
			//MeshComp->SetRelativeLocation(CostumeData->CostumePos);
			//MeshComp->SetRelativeRotation(FRotator::ZeroRotator); // 필요하면 CostumePivot 반영
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("Invalid Costume Data or Mesh"));
	}
}

USkeletalMesh* UBaseGameInstance::GetCostumeMesh(APawn* _Pawn)
{
	const FCostumeDataRow* CostumeData = UGlobalDataTable::GetCostumeData(_Pawn->GetWorld(), GetSelectedCostume());
	if (CostumeData && CostumeData->CostumeMesh)
	{
		return CostumeData->CostumeMesh;
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("Invalid Costume Data or Mesh"));
	}

	return nullptr;
}