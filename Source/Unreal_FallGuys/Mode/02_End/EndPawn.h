// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include <Global/GlobalEnum.h>

#include "EndPawn.generated.h"


UCLASS()
class UNREAL_FALLGUYS_API AEndPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEndPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// 이현정 : 코스튬 세팅
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* UpComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LowComp;

	UPROPERTY()
	class UStaticMesh* CurUpStaticMesh;

	UPROPERTY()
	class UStaticMesh* CurLowStaticMesh;

public:
	// 게임 인스로부터 승리한 플레이어의 정보를 얻어옴
	void InitDirectly(const FString& _Nick, const FString& _Color, const FString& _Top, const FString& _Bot);

protected:
	// 닉네임
	UPROPERTY()
	FString NickName = TEXT("");
	// 코스튬 컬러
	UPROPERTY()
	FString CostumeColor = TEXT("");
	// 코스튬 상의
	UPROPERTY()
	FString CostumeTop = TEXT("");
	// 코스튬 하의
	UPROPERTY()
	FString CostumeBot = TEXT("");
};
