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

protected:
	// 코스튬 컬러
	FString CostumeColor = TEXT("");
	// 코스튬 상의
	FString CostumeTop = TEXT("");
	// 코스튬 하의
	FString CostumeBot = TEXT("");

};
