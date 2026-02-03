// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuWidget.h"

#include "UI/TutorialWidget.h"


void UMenuWidget::SetSubWidget(UTutorialWidget* Widget)
{
	TutorialWidget = Widget;
}


void UMenuWidget::OnDirectionalInput(int32 Input)
{

}

void UMenuWidget::OnSelect()
{
	switch (CurrentMenuCount)
	{
		case 0: //resume
			break;
		case 1: //ContMap
			break;
		case 2: //Tutorial
			TutorialWidget->SetVisibility(ESlateVisibility::Visible);
			break;
		case 3: //Exit
			break;
		default:
		 break;

	}
}

//Press B Button
void UMenuWidget::CloseMenu()
{
	OnMenuClose.Broadcast();
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuHighlights = { ResumeHighlight, ContMapHighlight, TutorialHighlight, ExitHighlight };
}
