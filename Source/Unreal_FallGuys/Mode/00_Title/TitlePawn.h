// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Global/GlobalEnum.h>
#include "TitlePawn.generated.h"


UCLASS()
class UNREAL_FALLGUYS_API ATitlePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATitlePawn();

	UFUNCTION(BlueprintCallable)
	void PawnRotation(UStaticMeshComponent* _Target, const FVector2D& _Value);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PawnRotate = nullptr;

//LMH
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* UpComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LowComp;

	UPROPERTY()
	class UStaticMesh* CurUpStaticMesh;

	UPROPERTY()
	class UStaticMesh* CurLowStaticMesh;

	UFUNCTION()
	void AttachCustomStaticMesh(ECostumeType Type, FString& _ImgName);

	UFUNCTION()
	void DeAttachCustomStaticMesh(ECostumeType Type, FString& _ImgName);


};
