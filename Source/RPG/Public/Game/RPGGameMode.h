// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RPGGameMode.generated.h"

class UTutorialWidget;

// Dialogue ╫ц╫╨еш
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueEvent, const FString&, DialogueName, const FString&, DialogueLine);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterSaloon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitSaloon);

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

	//Callback Func called by tutorialWidget
	UFUNCTION()
	void OnTutorialClosed();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTutorialWidget> TutorialUIClass;  

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueEvent OnDialogueEvent;

	UPROPERTY()
	FOnEnterSaloon OnEnterSaloon;

	UPROPERTY()
	FOnExitSaloon OnExitSaloon;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void BroadcastDialogue(const FString& DialogueName, const FString& DialogueLine);

private:
	UTutorialWidget* TutorialWidget;
	
};
