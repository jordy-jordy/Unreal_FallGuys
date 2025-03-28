// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/00_Title/UI/CustomButtonWidget.h"
#include "Mode/00_Title/TitlePawn.h"

void UCustomButtonWidget::SetButtonStyle(UMaterialInterface* Mat)
{
	//FText Path = FText::FromString(_Path);

	//Mat = LoadObject<UMaterial>(nullptr, *_Path);

	if (Mat)
	{
		//FButtonStyle ButtonStyle = Button->GetStyle();
		//ButtonStyle.Normal.SetResourceObject(Texture);
		//Button->SetStyle(ButtonStyle);

		Img_View->SetBrushResourceObject(Mat);

		FSlateBrush Brush = Img_View->GetBrush();
		Brush.SetResourceObject(Mat);
		Img_View->SetBrush(Brush);

	}
}
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
//void UCustomButtonWidget::SetButtonStyle(UTexture2D* Texture, FString _Path)
//{
//	FText Path = FText::FromString(_Path);
//
//	Texture = LoadObject<UTexture2D>(nullptr, *_Path);
//
//	if (Texture)
//	{
//		//FButtonStyle ButtonStyle = Button->GetStyle();
//		//ButtonStyle.Normal.SetResourceObject(Texture);
//		//Button->SetStyle(ButtonStyle);
//
//		Img_View->SetBrushResourceObject(Texture);
//
//		FSlateBrush Brush = Img_View->GetBrush();
//		Brush.SetResourceObject(Texture);
//		Img_View->SetBrush(Brush);
//
//	}
//}

void UCustomButtonWidget::AttachCustomStaticMesh()
{

	//static mesh load

	ATitlePawn* MyPawn = Cast<ATitlePawn>(GetOwningPlayerPawn());


	MyPawn->AttachCustomStaticMesh(Type, ImgName);
}

void UCustomButtonWidget::DettachCustomStaticMesh()
{
	ATitlePawn* MyPawn = Cast<ATitlePawn>(GetOwningPlayerPawn());


	MyPawn->DeAttachCustomStaticMesh(Type, ImgName);
}

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