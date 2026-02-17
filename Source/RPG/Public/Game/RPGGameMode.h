// Manages game rules: dialogue events, save triggers, player respawn, BGM switching
// Singleplayer - focused: delegates UI / game state changes to PlayerCharacter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RPGGameMode.generated.h"

class UTutorialWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueEvent, const FString&, DialogueName, const FString&, DialogueLine);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveRequired);

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

	//Triggers dialogue UI in Player HUD
	UPROPERTY()
	FOnDialogueEvent OnDialogueEvent;

	UPROPERTY()
	FOnSaveRequired OnSaveRequired;

	UFUNCTION()
	void BroadcastDialogue(const FString& DialogueName, const FString& DialogueLine);

private:
	UTutorialWidget* TutorialWidget;
	
};
