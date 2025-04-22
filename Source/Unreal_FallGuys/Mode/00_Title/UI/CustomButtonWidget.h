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

	// 버튼 스타일 세팅
	void SetButtonStyle(UTexture2D* ImgTexture);
	void SetButtonStyle(UMaterialInterface* ImgMat);


	// 캐릭터 커스텀 관련 
	UFUNCTION()
	void AttachCustomStaticMesh();
	UFUNCTION()
	void DettachCustomStaticMesh();

	UFUNCTION(BlueprintCallable)
	void CustomStaticMesh();

	//변수
	// 위젯과 연결된 버튼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* Button;

	// 위젯과 연결된 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* Img_View;

	// 커스텀 이미지 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	FString ImgName = TEXT("");

	// 커스텀 상의 하의 구분
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	ECostumeType Type = ECostumeType::TOP;


};
