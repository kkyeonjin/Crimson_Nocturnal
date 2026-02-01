// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TutorialWidget.h"
#include "Components/Image.h"

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TutorialImages = { Page1, Page2, Page3, Page4, Page5, Page6 };
}
