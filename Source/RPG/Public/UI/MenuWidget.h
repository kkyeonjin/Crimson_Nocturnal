// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIInputWidget.h"
#include "MenuWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuClose);

class UImage;
class UControllerMapWidget;

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

	//Sub Widget
	UPROPERTY(EditAnywhere)
	UControllerMapWidget* ControllerMapWidget;

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

	UFUNCTION(BlueprintCallable)
	void SetSubWidget(UTutorialWidget* TutoWidget, UControllerMapWidget* ContWidget);

	void OnDirectionalInput(int32 Input);

	void OnSelect();

	void CloseMenu();

protected:
	virtual void OnDPadUp() override { OnDirectionalInput(1); }
	virtual void OnDPadDown() override { OnDirectionalInput(-1); }
	virtual void OnFaceButtonA() override { OnSelect(); }
	virtual void OnFaceButtonB() override { CloseMenu(); }


private:
	TArray<UImage*> MenuHighlights;

	int32 CurrentMenuCount = 0;

	
};
