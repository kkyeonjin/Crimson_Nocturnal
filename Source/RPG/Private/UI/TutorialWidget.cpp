// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TutorialWidget.h"
#include "Components/Image.h"


void UTutorialWidget::TurnPages(int32 Input)
{
	TutorialImages[CurrentPage]->SetVisibility(ESlateVisibility::Hidden);

	CurrentPage += Input;
	if (CurrentPage == -1)
	{
		CurrentPage = 5;
	}
	else if (CurrentPage == 6)
	{
		CurrentPage = 0;
	}

	TutorialImages[CurrentPage]->SetVisibility(ESlateVisibility::Visible);
}

void UTutorialWidget::CloseTutorial()
{
	OnTutorialClose.Broadcast();
}

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TutorialImages = { Page1, Page2, Page3, Page4, Page5, Page6 };
}
