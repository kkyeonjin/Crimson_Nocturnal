#include "Component/Player/InteractionComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/ShapeComponent.h"
#include "Interface/InteractableInterface.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
    if (!PlayerCharacter) return;
}

void UInteractionComponent::InitializeCapsuleCollision(UShapeComponent* NewPlayerCapsule)
{
    PlayerCapsule = NewPlayerCapsule;

    PlayerCapsule->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnCollisionOverlap);
    PlayerCapsule->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnCollisionEndOverlap);
}

void UInteractionComponent::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Portal") || OtherActor->ActorHasTag("NPC"))
    {
        IInteractable* InteractableInterface = Cast<IInteractable>(OtherActor);

        if (InteractableInterface)
        {
            UE_LOG(LogTemp, Warning, TEXT("Portal / NPC Detected"));

            CurrentInteractable = OtherActor;

            //for interact pop up
            PlayerCharacter->InteractPopUp();
        }
        else
        {
            PlayerCharacter->HideInteractPopUp();
            CurrentInteractable = nullptr;
        }
    }
    else if (OtherActor->ActorHasTag("DanceFloor"))
    {
        UE_LOG(LogTemp, Warning, TEXT("Dance Floor Collision Overlapped"));

        CurrentInteractable = OtherActor;

        PlayerCharacter->DancePopUp();
    }
}

void UInteractionComponent::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    PlayerCharacter->HideInteractPopUp();
    CurrentInteractable = nullptr;
}

void UInteractionComponent::InteractionStart()
{
    if (CurrentInteractable)
    {
        IInteractable* InteractableInterface = Cast<IInteractable>(CurrentInteractable);

        if (InteractableInterface)
        {
            InteractableInterface->OnInteract();
        }
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("CurrentInteractable NOT FOUND!!!"));
    }
}
