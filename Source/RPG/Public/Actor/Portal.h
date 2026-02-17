// Portal that helps move to the another Level
// Teleport enables when interact with portal

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Portal.generated.h"

UCLASS()
class RPG_API APortal : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    APortal();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* PortalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
    FName DestinationMap;

    // IInteractable
    virtual void OnInteract() override;

private:
    // Check ture when the portal is in Saloon
	UPROPERTY(EditAnywhere)
    bool ExitingSaloon = false;  
   
};