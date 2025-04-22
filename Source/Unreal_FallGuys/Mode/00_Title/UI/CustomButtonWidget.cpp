// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomButtonWidget.h"
#include "Mode/00_Title/TitlePawn.h"

// 버튼에 메테리얼 지정
void UCustomButtonWidget::SetButtonStyle(UMaterialInterface* Mat)
{
	if (Mat)
	{
		Img_View->SetBrushResourceObject(Mat);

		FSlateBrush Brush = Img_View->GetBrush();
		Brush.SetResourceObject(Mat);
		Img_View->SetBrush(Brush);
	}
}

//버튼에 텍스쳐 입히기
void UCustomButtonWidget::SetButtonStyle(UTexture2D* ImgTexture)
{
	if (ImgTexture)
	{
		Img_View->SetBrushResourceObject(ImgTexture);

		FSlateBrush Brush = Img_View->GetBrush();
		Brush.SetResourceObject(ImgTexture);
		Img_View->SetBrush(Brush);
	}
}

// 타이틀폰에 커스텀 메쉬 입히기
void UCustomButtonWidget::AttachCustomStaticMesh()
{

	ATitlePawn* MyPawn = Cast<ATitlePawn>(GetOwningPlayerPawn());


	MyPawn->AttachCustomStaticMesh(Type, ImgName);
}
// 타이틀폰 커스텀 메쉬 없애기
void UCustomButtonWidget::DettachCustomStaticMesh()
{
	ATitlePawn* MyPawn = Cast<ATitlePawn>(GetOwningPlayerPawn());


	MyPawn->DeAttachCustomStaticMesh(Type, ImgName);
}

// 이미지 이름으로 커스텀 하기
void UCustomButtonWidget::CustomStaticMesh()
{
	if (ImgName == "")
	{
		DettachCustomStaticMesh();
	}
	else
	{
		AttachCustomStaticMesh();
	}
}