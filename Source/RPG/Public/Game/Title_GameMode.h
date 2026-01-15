// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Title_GameMode.generated.h"

class UTitleWidget;
/**
 * 
 */
UCLASS()
class RPG_API ATitle_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTitleWidget> TitleWidgetClass;

	UPROPERTY()
	UTitleWidget* TitleWidget;
	
};
