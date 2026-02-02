// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class RPG_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* Page1;

	UPROPERTY(meta = (BindWidget))
	UImage* Page2;

	UPROPERTY(meta = (BindWidget))
	UImage* Page3;

	UPROPERTY(meta = (BindWidget))
	UImage* Page4;

	UPROPERTY(meta = (BindWidget))
	UImage* Page5;

	UPROPERTY(meta = (BindWidget))
	UImage* Page6;

	void TurnPages(int32 Input);

	void CloseTutorial();

private:
	TArray<UImage*> TutorialImages;

	int32 CurrentPage = 0;
	
};
