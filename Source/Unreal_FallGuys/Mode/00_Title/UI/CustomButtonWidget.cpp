// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomButtonWidget.h"
#include "Mode/00_Title/TitlePawn.h"

// ��ư�� ���׸��� ����
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

//��ư�� �ؽ��� ������
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

// Ÿ��Ʋ���� Ŀ���� �޽� ������
void UCustomButtonWidget::AttachCustomStaticMesh()
{

	ATitlePawn* MyPawn = Cast<ATitlePawn>(GetOwningPlayerPawn());


	MyPawn->AttachCustomStaticMesh(Type, ImgName);
}
// Ÿ��Ʋ�� Ŀ���� �޽� ���ֱ�
void UCustomButtonWidget::DettachCustomStaticMesh()
{
	ATitlePawn* MyPawn = Cast<ATitlePawn>(GetOwningPlayerPawn());


	MyPawn->DeAttachCustomStaticMesh(Type, ImgName);
}

// �̹��� �̸����� Ŀ���� �ϱ�
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