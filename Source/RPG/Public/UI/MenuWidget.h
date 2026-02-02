// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class RPG_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	//Sub Widget
	UPROPERTY(EditAnywhere)
	class UTutorialWidget* TutorialWidget;


	UPROPERTY(meta = (BindWidget))
	UImage* ResumeHighlight;

	UPROPERTY(meta = (BindWidget))
	UImage* ContMapHighlight;

	UPROPERTY(meta = (BindWidget))
	UImage* TutorialHighlight;

	UPROPERTY(meta = (BindWidget))
	UImage* ExitHighlight;

	UPROPERTY(meta = (BindWidget))
	UImage* ControllerMap;

	UFUNCTION(BlueprintCallable)
	void SetSubWidget(UTutorialWidget* Widget);

	void OnDirectionalInput(int32 Input);

	void BackToPrev();


private:
	TArray<UImage*> MenuHighlights;

	int32 CurrentMenuCount = 0;

	
};
