// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GroundSlam.h"
#include "Component/Common/EnemyAttackComponent.h"

// Sets default values
AGroundSlam::AGroundSlam()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGroundSlam::BeginPlay()
{
    Super::BeginPlay();

    CachedAttackComponent = FindComponentByClass<UEnemyAttackComponent>();
    if (!CachedAttackComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("BossAttackComp Finding Failed in BeginPlay"));
    }

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

        
        GetWorld()->GetTimerManager().SetTimer(
            CollisionStartTimer,
            this,
            &AGroundSlam::CollisionEnable,
            0.7f,
            false
        );
    }
}

//콜리전 활성화 위한 함수(추후 리네이밍 필요)
void AGroundSlam::CollisionEnable()
{
    for (auto Actor : SpawnedLightningActors)
    {
        UEnemyAttackComponent* AttackComp = Actor->FindComponentByClass<UEnemyAttackComponent>();
        AttackComp->OnAttackCollisionEnable(-1);
    }
}


