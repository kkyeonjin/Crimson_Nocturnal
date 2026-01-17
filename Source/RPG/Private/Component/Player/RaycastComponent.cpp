// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/RaycastComponent.h"

#include "Character/Player/PlayerCharacter.h"
#include "Components/ShapeComponent.h"
#include "Interface/InteractableInterface.h"

URaycastComponent::URaycastComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void URaycastComponent::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
    if (!PlayerCharacter) return;
}

void URaycastComponent::InitializeCapsuleCollision(UShapeComponent* NewPlayerCapsule)
{
    PlayerCapsule = NewPlayerCapsule;

    PlayerCapsule->OnComponentBeginOverlap.AddDynamic(this, &URaycastComponent::OnCollisionOverlap);
}

void URaycastComponent::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Portal") || OtherActor->ActorHasTag("NPC"))
    {
        UE_LOG(LogTemp, Warning, TEXT("Ray Collision Overlapped"));

        FVector Start = PlayerCharacter->GetActorLocation() + FVector(0, 0, 100);
        FVector End = Start + PlayerCharacter->GetActorForwardVector() * TraceDistance;

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(PlayerCharacter);

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_WorldDynamic,
            QueryParams
        );

        if (HitResult.GetActor())
        {
            IInteractable* InteractableInterface = Cast<IInteractable>(HitResult.GetActor());

            if (InteractableInterface)
            {
                UE_LOG(LogTemp, Warning, TEXT("Portal / NPC Detected"));

                //InteractableInterface->OnInteract();
            }
        }

    }
}


