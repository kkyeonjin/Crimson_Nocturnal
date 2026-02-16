// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIInputWidget.h"
#include "EndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UEndingWidget : public UBaseUIInputWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	virtual void OnFaceButtonB() override { EndGame(); }

private:
	void EndGame();
	
};
