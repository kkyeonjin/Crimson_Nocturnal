// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define MENUNUM  3

#include "CoreMinimal.h"
#include "UI/BaseUIInputWidget.h"
#include "TitleWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class RPG_API UTitleWidget : public UBaseUIInputWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override; //BeginPlay같은거, 초기세팅 

	UPROPERTY(meta = (BindWidget))
	UImage* StartCircle;

	UPROPERTY(meta = (BindWidget))
	UImage* ContinueCircle;

	UPROPERTY(meta = (BindWidget))
	UImage* ExitCircle;

protected:
	// Title은 정말 최소한만!
	virtual void OnDPadUp() override { OnNavigate(1); }
	virtual void OnDPadDown() override { OnNavigate(-1); }
	virtual void OnFaceButtonA() override { OnSelect(); }

private:

	int32 TitleMenuSelectCount = 0;

	TArray<UImage*> MenuCircles;

	void OnNavigate(int32 Input);
	void OnSelect();
};
