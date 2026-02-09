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

        FVector Start = PlayerCharacter->GetActorLocation() + FVector(0, 0, 50);
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

        // LineTrace 전에/후에 추가 (빨간색 선, 5초 지속, 두께 2.0f)
        DrawDebugLine(
            GetWorld(),
            Start,
            End,
            FColor::Red,      // 색상 (Hit 여부에 따라 변경 가능)
            false,            // Persistent 여부
            5.0f,             // 지속 시간 (0이면 한 프레임)
            0,                // Depth Priority
            2.0f              // 두께
        );

        // Hit 시 끝점 변경 (Hit 지점까지 선 그리기)
        if (bHit)
        {
            DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Green, false, 5.0f, 0, 3.0f);
            DrawDebugSphere(GetWorld(), HitResult.Location, 20.0f, 12, FColor::Green, false, 5.0f);  // Hit 지점 구체
        }

        if (HitResult.GetActor())
        {
            IInteractable* InteractableInterface = Cast<IInteractable>(HitResult.GetActor());

            if (InteractableInterface)
            {
                UE_LOG(LogTemp, Warning, TEXT("Portal / NPC Detected"));

                //for interact pop up
                OnRaycastDetected.Broadcast();

                InteractableInterface->OnInteract();
            }
        }
    }
}


