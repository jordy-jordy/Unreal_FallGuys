// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Global/GlobalEnum.h>
#include "CustomButtonWidget.generated.h"


/**
 * 
 */
UCLASS()
class UNREAL_FALLGUYS_API UCustomButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:

	// ��ư ��Ÿ�� ����
	void SetButtonStyle(UTexture2D* ImgTexture);
	void SetButtonStyle(UMaterialInterface* ImgMat);


	// ĳ���� Ŀ���� ���� 
	UFUNCTION()
	void AttachCustomStaticMesh();
	UFUNCTION()
	void DettachCustomStaticMesh();

	UFUNCTION(BlueprintCallable)
	void CustomStaticMesh();

	//����
	// ������ ����� ��ư
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* Button;

	// ������ ����� �̹���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* Img_View;

	// Ŀ���� �̹��� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	FString ImgName = TEXT("");

	// Ŀ���� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	ECostumeType Type = ECostumeType::TOP;


};
