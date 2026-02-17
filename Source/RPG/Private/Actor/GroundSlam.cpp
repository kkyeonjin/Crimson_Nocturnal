#include "Actor/GroundSlam.h"
#include "Component/Common/EnemyAttackComponent.h"

AGroundSlam::AGroundSlam()
{
     PrimaryActorTick.bCanEverTick = false;
}

void AGroundSlam::BeginPlay()
{
    Super::BeginPlay();

    StartSlam();
}

void AGroundSlam::StartSlam()
{
    if (GetWorld())
    {
        for (const FVector& Point : SpawnPoints)
        {
            AActor* Spawned = GetWorld()->SpawnActor<AActor>(
                LighteningActor,
                Point,
                FRotator::ZeroRotator);

            if (Spawned)
            {
                SpawnedLightningActors.Add(Spawned);
            }
        }


        // Delayed collision enable
        // cause lightening particle arrives at ground after few seconds 
        GetWorld()->GetTimerManager().SetTimer(
            CollisionStartTimer,
            this,
            &AGroundSlam::CollisionEnable,
            0.1f,
            false
        );
    }
}

void AGroundSlam::CollisionEnable()
{
    for (auto Actor : SpawnedLightningActors)
    {
        UEnemyAttackComponent* AttackComp = Actor->FindComponentByClass<UEnemyAttackComponent>();
        AttackComp->OnAttackCollisionEnable(-1);
    }
}


