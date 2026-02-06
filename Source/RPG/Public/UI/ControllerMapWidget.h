// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUIInputWidget.h"
#include "ControllerMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UControllerMapWidget : public UBaseUIInputWidget
{
	GENERATED_BODY()

public:
	void InitializeContMap();

protected:
	virtual void OnFaceButtonB() override { CloseContMap(); }

	void CloseContMap();
	
};
