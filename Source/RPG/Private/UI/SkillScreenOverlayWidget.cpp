// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillScreenOverlayWidget.h"
#include "Components/Image.h"



void USkillScreenOverlayWidget::ShowSkillEffect(int32 SkillNum)
{
	SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogTemp, Display, TEXT("ShowSkillScreenOverlay"));
	SkillImages[SkillNum]->SetVisibility(ESlateVisibility::Visible);
	
}

void USkillScreenOverlayWidget::HideSkillEffect()
{
	SetVisibility(ESlateVisibility::Hidden);
}


void USkillScreenOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillImages = { BloodRush, HPDrain, Infect };
}
