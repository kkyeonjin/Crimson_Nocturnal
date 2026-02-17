#include "Actor/BossDoor.h"

ABossDoor::ABossDoor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABossDoor::BeginPlay()
{
    Super::BeginPlay();

    OpenPosition = this->GetActorLocation();
}

void ABossDoor::CloseDoor()
{
    if (bIsAnimating) return;

    bIsAnimating = true;
    DoorAnimationElapsedTime = 0.0f;

    FVector StartWorldPos = GetActorLocation();
    // ClosedPosition is offset
    FVector EndWorldPos = StartWorldPos + ClosedPosition;  

    GetWorld()->GetTimerManager().SetTimer(DoorTimer, [this, StartWorldPos, EndWorldPos]()
        {
            DoorAnimationElapsedTime += GetWorld()->DeltaTimeSeconds;
            float Alpha = FMath::Clamp(DoorAnimationElapsedTime / DoorAnimationDuration, 0.0f, 1.0f);

        // Linear interpolation
            FVector NewWorldPos = FMath::Lerp(StartWorldPos, EndWorldPos, Alpha);

            SetActorLocation(NewWorldPos);

            if (Alpha >= 1.0f)
            {
                GetWorld()->GetTimerManager().ClearTimer(DoorTimer);
                bIsAnimating = false;
            }
        }, GetWorld()->DeltaTimeSeconds, true);
}



