// NPC that can speak (Dialogue)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/InteractableInterface.h"
#include "NPC.generated.h"

class UDialogueComponent;

UCLASS()
class RPG_API ANPC : public ACharacter, public IInteractable
{
    GENERATED_BODY()

public:
    ANPC();

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* CollisionBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FString NPCName;

    // IInteractable
    virtual void OnInteract() override;

private:
    UDialogueComponent* DialogueComponent;

};
