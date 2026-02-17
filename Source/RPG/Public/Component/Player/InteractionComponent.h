// For interaction with NPC / Portal / DanceFloor
// Manage overlap with player's capsule collision & other Interactables

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

    UPROPERTY()
    AActor* CurrentInteractable = nullptr;

    UFUNCTION()
    void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void InteractionStart();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    UShapeComponent* PlayerCapsule;

    UFUNCTION(BlueprintCallable)
    void InitializeCapsuleCollision(UShapeComponent* NewPlayerCapsule);

private:
    APlayerCharacter* PlayerCharacter;
		
};
