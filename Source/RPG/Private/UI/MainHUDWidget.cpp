// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUDWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UMainHUDWidget::UpdateHP(int32 Current, int32 Max)
{
	float Percent = (float)Current / (float)Max;
	//UE_LOG(LogTemp, Error, TEXT("HP Percent: %f"), Percent);

	if (HPBar)
	{
		HPBar->SetPercent(Percent);
		UE_LOG(LogTemp, Error, TEXT("HPBarUpdate"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HPBarCANNOTFound"));
	}

	if (HPText)
	{
		HPText->SetText(FText::AsNumber(Current));
	}
}

void UMainHUDWidget::UpdateBP(int32 Current, int32 Max)
{
	float Percent = (float)Current / (float)Max;
	//UE_LOG(LogTemp, Error, TEXT("HP Percent: %f"), Percent);

	if (BPBar)
	{
		BPBar->SetPercent(Percent);
		UE_LOG(LogTemp, Error, TEXT("BPBarUpdate"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BPBarCANNOTFound"));
	}

	if (BPText)
	{
		BPText->SetText(FText::AsNumber(Current));
	}
}

void UMainHUDWidget::ShowQTEUI(int32 ComboNum)
{
	if (ComboQTEImages.Num() != 0)
	{
		ComboQTEImages[ComboNum]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainHUDWidget::HideQTEUI(int32 ComboNum)
{
	if (ComboQTEImages.Num() != 0)
	{
		ComboQTEImages[ComboNum]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ComboQTEImages = { ComboA, ComboB, ComboC, ComboD };

}

