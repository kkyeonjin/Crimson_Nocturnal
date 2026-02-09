// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();


protected:
	//Define in Editor 
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString DialogueName;

	//Define in Editor 
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString DialogueLine;

public:
	FORCEINLINE FString GetDialogueName() { return DialogueName; }
	FORCEINLINE FString GetDialogueLine() { return DialogueLine; }

};
