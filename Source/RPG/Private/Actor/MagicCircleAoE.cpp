// 


#include "Actor/MagicCircleAoE.h"
#include "Components/DecalComponent.h"
#include "Actor/MagicCircle.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMagicCircleAoE::AMagicCircleAoE()
{
     PrimaryActorTick.bCanEverTick = false;
}


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

    //Warning Timer Start 
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

// when phase1 ends
void AMagicCircleAoE::OnExplosionEnd()
{
    Destroy();
}

void AMagicCircleAoE::SetRandomPoints()
{
    //Initialize RandomPoints Array(Clear)
    RandomPoints.Empty();

    StageCenter = GetActorLocation();

    float AngleStep = 360.f / 5;

    for (int i = 0; i < 5; i++)
    {
        // Base angle for perfect pentagon arrangement
        float BaseAngle = AngleStep * i;

        // Add small random offset for organic feel (¡¾5¡Æ)
        float RandomOffset = FMath::RandRange(-5.f, 5.f);
        float FinalAngle = (BaseAngle + RandomOffset) * PI / 180.f;

        // Polar ¡æ Cartesian: radius 500 units, Z=50 (slightly above ground)
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

