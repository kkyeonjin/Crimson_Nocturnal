// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class IInteractable
{
    GENERATED_BODY()

public:
    //UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    //void OnInteract(ACharacter* Instigator);

    //UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    virtual void OnInteract() = 0;

    //UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    //FString GetInteractionPromptText() const;
};
