// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RPGGameMode.generated.h"

class UTutorialWidget;

/**
 * 
 */
UCLASS()
class RPG_API ARPGGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void ShowStandaloneTutorial();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTutorialWidget> TutorialUIClass;  

private:
	UTutorialWidget* TutorialWidget;
	
};
