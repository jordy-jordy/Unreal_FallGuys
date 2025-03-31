// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/ImMovable/JumpShowDown/ShowDownStage.h"

#include "Kismet/GameplayStatics.h" 
#include "Unreal_FallGuys.h"
#include "Mode/01_Play/PlayGameMode.h"

// Sets default values
AShowDownStage::AShowDownStage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShowDownStage::BeginPlay()
{
	Super::BeginPlay();
	
	SetPlatforms();
	
	/*if (nullptr != GetWorld()->GetAuthGameMode())
	{
		GetWorld()->GetAuthGameMode()->AddShowDown(this);
	}*/

	// TArray<AShowDownStage>

	// 이현정 : 플랫폼 등록
	RegisterPlatformsToGameMode();
}

// Called every frame
void AShowDownStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShowDownStage::SetPlatforms()
{
	UWorld* World = GetWorld();

	if (World && BP_Platforms)
	{
		for (int32 i = 0; i < 8; i++)
		{
			FVector SpawnLocation = FVector(-10.0f, -40.0f, 0.0f);
			FRotator SpawnRotation = FRotator(0, -135 + (i * 45), 0);
			AShowDownPlatform* NewPlatform = World->SpawnActor<AShowDownPlatform>(BP_Platforms, SpawnLocation, SpawnRotation);

			if (NewPlatform)
			{
				Platforms.Add(NewPlatform);
			}

			Platforms[i]->IsBlue = (i % 2 == 0);
			(Platforms[i]->IsBlue) ? NewPlatform->SetMesh_Blue() : NewPlatform->SetMesh_Orange();
		}
	}
}

// 원본 SetFallPlatform 함수
//void AShowDownStage::SetFallPlatform()
//{
//	int32 Num = FMath::RandRange(0, 7);
//
//	if (true == Platforms[Num]->IsLive)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("FallPlatform Number is : %d"), Num);
//		Platforms[Num]->SetActorLocation(GetActorLocation() + FVector(0, 0, -10));
//		Platforms[Num]->IsLive = false;
//	}
//	else
//	{
//		SetFallPlatform();
//	}
//}

// 이현정 : GetRandomPlatform 함수를 이용한 예시
void AShowDownStage::SetFallPlatform()
{
	UWorld* World = GetWorld();
	if (!World) return;

	// 게임 모드를 가져옴
	APlayGameMode* GameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(World));
	if (!GameMode)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("SetFallPlatform :: GameMode 캐스팅 실패"));
		return;
	}

	// 게임 모드의 함수를 이용해 랜덤 플랫폼을 얻어옴
	AShowDownPlatform* Platform = GameMode->GetRandomPlatform();
	if (Platform)
	{
		// 해당 플랫폼의 위치를 조정하는 방식
		Platform->SetActorLocation(Platform->GetActorLocation() + FVector(0, 0, -10));
		UE_LOG(FALL_DEV_LOG, Log, TEXT("SetFallPlatform :: 플랫폼 낙하 처리 완료"));
	}
	else
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("SetFallPlatform :: 유효한 플랫폼이 없음"));
	}
}

// 이현정 : 생성된 플랫폼을 게임 모드에 등록하는 함수
void AShowDownStage::RegisterPlatformsToGameMode()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayGameMode* GameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(World));
	if (!GameMode)
	{
		UE_LOG(FALL_DEV_LOG, Warning, TEXT("ShowDownStage :: RegisterPlatformsToGameMode :: GameMode가 nullptr 입니다."));
		return;
	}

	for (AShowDownPlatform* Platform : Platforms)
	{
		if (IsValid(Platform))
		{
			GameMode->AddShowDownPlatform(Platform);
			UE_LOG(FALL_DEV_LOG, Log, TEXT("ShowDownStage :: RegisterPlatformsToGameMode :: 플랫폼이 등록되었습니다."));
		}
	}
}