// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MagicCircleAoE.h"
#include "Components/DecalComponent.h"
#include "Actor/MagicCircle.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMagicCircleAoE::AMagicCircleAoE()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMagicCircleAoE::BeginPlay()
{
    Super::BeginPlay();

    InitializeCircle();
}


void AMagicCircleAoE::InitializeCircle()
{

    SetRandomPoints();

    //1. Spawn MagicCircle Decal at Random Points
    if (DecalClass && GetWorld())
    {
        for (const FVector& Point : RandomPoints)
        {
            AMagicCircle* DecalActor = GetWorld()->SpawnActor<AMagicCircle>(
                DecalClass,
                Point,
                FRotator::ZeroRotator
            );
        }
    }

    //Warning TImer Start 
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            WarningTimerHandle,
            this,
            &AMagicCircleAoE::OnWarningEnd,
            WarningDuration,
            false
        );
    }
}

void AMagicCircleAoE::OnWarningEnd()
{
    //2. Spawn Fire Storm Actors at Random Points
    if (ParticleEffectClass && GetWorld())
    {
        for (const FVector& Point : RandomPoints)
        {
            AActor* ParticleActor = GetWorld()->SpawnActor<AActor>(
                ParticleEffectClass,
                Point,
                FRotator::ZeroRotator
            );
        }
    }
}

void AMagicCircleAoE::OnExplosionEnd()
{
    Destroy();
    UE_LOG(LogTemp, Display, TEXT("ActorDestroyed"));
}

void AMagicCircleAoE::SetRandomPoints()
{
    //Initialize RandomPoints Array(Clear)
    RandomPoints.Empty();

    StageCenter = GetActorLocation();

    float AngleStep = 360.f / 5;

    for (int i = 0; i < 5; i++)
    {
        float BaseAngle = AngleStep * i;

        float RandomOffset = FMath::RandRange(-5.f, 5.f);
        float FinalAngle = (BaseAngle + RandomOffset) * PI / 180.f;

        //float Distance = FMath::RandRange(MagicCircleRadius * 0.75f, MagicCircleRadius * 0.95f);

        FVector RandomPoint = StageCenter + FVector(
            FMath::Cos(FinalAngle) * 500.f,
            FMath::Sin(FinalAngle) * 500.f,
            50.f
        );

        RandomPoints.Add(RandomPoint);
    }

    //Random Points Array is set
    RandomPoints.Add(GetActorLocation());
}

