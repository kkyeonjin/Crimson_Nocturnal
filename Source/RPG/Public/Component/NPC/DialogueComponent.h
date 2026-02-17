// Created for BP of this Component 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();


protected:
	//Define in Editor for different dialogue in each NPC
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString DialogueName;

	//Define in Editor for different dialogue in each NPC
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString DialogueLine;

public:
	FORCEINLINE FString GetDialogueName() { return DialogueName; }
	FORCEINLINE FString GetDialogueLine() { return DialogueLine; }

};
