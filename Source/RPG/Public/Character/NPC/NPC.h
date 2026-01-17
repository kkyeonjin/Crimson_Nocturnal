// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/InteractableInterface.h"
#include "NPC.generated.h"

UCLASS()
class RPG_API ANPC : public ACharacter, public IInteractable
{
    GENERATED_BODY()

public:
    ANPC();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* CollisionBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FString NPCName;

    // IInteractable ±¸Çö
    virtual void OnInteract() override;

private:

};
