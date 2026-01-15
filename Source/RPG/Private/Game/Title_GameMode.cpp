// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Title_GameMode.h"
#include "UI/TitleWidget.h"

void ATitle_GameMode::BeginPlay()
{
	Super::BeginPlay();

	if (TitleWidgetClass)
	{
		TitleWidget = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
		if (TitleWidget)
		{
			TitleWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MainHUDWidgetNotFound"));
		}
	}
}
