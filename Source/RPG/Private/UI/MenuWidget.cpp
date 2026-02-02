// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuWidget.h"


void UMenuWidget::SetSubWidget(UTutorialWidget* Widget)
{
	TutorialWidget = Widget;
}


void UMenuWidget::OnDirectionalInput(int32 Input)
{

}

//Press B Button
void UMenuWidget::BackToPrev()
{

}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuHighlights = { ResumeHighlight, ContMapHighlight, TutorialHighlight, ExitHighlight };
}
