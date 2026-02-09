// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BossDoor.h"

ABossDoor::ABossDoor()
{
    PrimaryActorTick.bCanEverTick = false;

    // 기본 위치 저장 (생성 시점)
    //OpenPosition = FVector(0, 0, 0);
}

void ABossDoor::BeginPlay()
{
    Super::BeginPlay();

    // 현재 위치를 OpenPosition으로 저장
    OpenPosition = this->GetActorLocation();

    if (bDebugLog)
    {
        UE_LOG(LogTemp, Warning, TEXT("BossDoor OpenPos: %s"), *OpenPosition.ToString());
        UE_LOG(LogTemp, Warning, TEXT("BossDoor ClosedPos: %s"), *ClosedPosition.ToString());
    }
}

void ABossDoor::CloseDoor()
{
    if (bIsAnimating) return;

    bIsAnimating = true;
    DoorAnimationElapsedTime = 0.0f;

    FVector StartWorldPos = GetActorLocation();
    FVector EndWorldPos = StartWorldPos + ClosedPosition;  // 현재 위치 + 오프셋

    GetWorld()->GetTimerManager().SetTimer(DoorTimer, [this, StartWorldPos, EndWorldPos]()
        {
            DoorAnimationElapsedTime += GetWorld()->DeltaTimeSeconds;
            float Alpha = FMath::Clamp(DoorAnimationElapsedTime / DoorAnimationDuration, 0.0f, 1.0f);

            FVector NewWorldPos = FMath::Lerp(StartWorldPos, EndWorldPos, Alpha);
            SetActorLocation(NewWorldPos);  // 액터 전체 이동

            if (Alpha >= 1.0f)
            {
                GetWorld()->GetTimerManager().ClearTimer(DoorTimer);
                bIsAnimating = false;
                UE_LOG(LogTemp, Display, TEXT("BossDoor CLOSED at %s"), *NewWorldPos.ToString());
            }
        }, GetWorld()->DeltaTimeSeconds, true);
}



