// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ControllerMapWidget.h"

void UControllerMapWidget::InitializeContMap()
{
	this->SetVisibility(ESlateVisibility::Visible);
	this->SetKeyboardFocus();
}

void UControllerMapWidget::CloseContMap()
{
	UE_LOG(LogTemp, Display, TEXT("ContMapClose"));
	this->SetVisibility(ESlateVisibility::Hidden);
}
