// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUDWidget.h"
#include "Components/ProgressBar.h"

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
	
}

void UMainHUDWidget::UpdateBP(int32 Current, int32 Max)
{
	float Percent = Current / Max;

	BPBar->SetPercent(Percent);
}
