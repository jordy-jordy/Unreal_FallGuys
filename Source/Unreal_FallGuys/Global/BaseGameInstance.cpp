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

			PlayLevelDataTable = DataTables->FindRow<FDataTableRow>("DT_PlayLevelDataTable", nullptr)->Resources;
			if (nullptr == PlayLevelDataTable)
			{
				UE_LOG(FALL_DEV_LOG, Error, TEXT("%S(%u)> if (nullptr == PlayLevelDataTable)"), __FUNCTION__, __LINE__);
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
	// ���
	UE_LOG(FALL_DEV_LOG, Log, TEXT("%S(%u)> DataTableLoading End"), __FUNCTION__, __LINE__);
}

// ���� ����
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

// ���� ���� : ���� ���� �ʿ�
void UBaseGameInstance::InsSelectedServerStart(UWorld* _World, FString _Port, FString _OpenLevel)
{
	if (!_World)
	{
		UE_LOG(FALL_DEV_LOG, Error, TEXT("CServerStart: _World is nullptr"));
		return;
	}

	FString OpenLevel;
	FString LevelPath = TEXT("");

	UFallGlobal::AssetPackagePath(UWorld::StaticClass(), _OpenLevel, LevelPath);
	OpenLevel = FString::Printf(TEXT(":%s%s"), *_Port, *LevelPath);

	UGameplayStatics::OpenLevel(_World, *OpenLevel, true, TEXT("listen"));
}

// ���� ����
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

// ����ȭ ����
void UBaseGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseGameInstance, CostumeName);
	DOREPLIFETIME(UBaseGameInstance, Nickname);
}

// �ڽ�Ƭ �̸� ����
void UBaseGameInstance::InsSaveCostumeName(const FString& _CostumeName)
{
	CostumeName = _CostumeName;
	UE_LOG(FALL_DEV_LOG, Warning, TEXT("����� �ڽ�Ƭ �̸� : %s"), *CostumeName);
}

// Pawn�� �ڽ�Ƭ ����
void UBaseGameInstance::InsChangeCostume(APawn* _Pawn, const FString& _CostumeName)
{
	UWorld* World = _Pawn->GetWorld();
	const FCostumeDataRow* CostumeData = UGlobalDataTable::GetCostumeData(World, _CostumeName);
	if (CostumeData && CostumeData->CostumeMesh)
	{
		// _Pawn�� ���̷�Ż �޽� ��������
		if (USkeletalMeshComponent* MeshComp = _Pawn->FindComponentByClass<USkeletalMeshComponent>())
		{
			MeshComp->SetSkeletalMesh(CostumeData->CostumeMesh);
		}
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("ChangeCostume :: Invalid Costume Data or Mesh"));
	}

	InsSaveCostumeName(_CostumeName);
}

// ����� �ڽ�Ƭ�� ���̷�Ż �޽� ��ȯ
USkeletalMesh* UBaseGameInstance::InsGetCostumeMesh(APawn* _Pawn, const FString& _MeshName/* = TEXT("NULL")*/)
{
	const FCostumeDataRow* CostumeData = UGlobalDataTable::GetCostumeData(_Pawn->GetWorld(), _MeshName);
	if (CostumeData && CostumeData->CostumeMesh)
	{
		return CostumeData->CostumeMesh;
	}
	else
	{
		if (_MeshName.IsEmpty())
		{
			UE_LOG(FALL_DEV_LOG, Warning, TEXT("GetCostumeMesh :: Empty Costume Data or Mesh"));
		}
		else
		{
			UE_LOG(FALL_DEV_LOG, Error, TEXT("GetCostumeMesh :: Invalid Costume Data or Mesh"));
		}
	}

	return nullptr;
}

// �г��� ����
void UBaseGameInstance::InsChangeNickname(const FString& _NewNickname)
{
	Nickname = _NewNickname;
}
