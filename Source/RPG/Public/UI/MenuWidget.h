// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIInputWidget.h"
#include "MenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuClose);

class UImage;

/**
 * 
 */
UCLASS()
class RPG_API UMenuWidget : public UBaseUIInputWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	//Sub Widget
	UPROPERTY(EditAnywhere)
	class UTutorialWidget* TutorialWidget;

	UPROPERTY()
	FOnMenuClose OnMenuClose;

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

	void OnSelect();

	void CloseMenu();

protected:
	virtual void OnDPadUp() override { OnDirectionalInput(1); }
	virtual void OnDPadDown() override { OnDirectionalInput(-1); }
	virtual void OnFaceButtonA() override {  }


private:
	TArray<UImage*> MenuHighlights;

	int32 CurrentMenuCount = 0;

	
};
