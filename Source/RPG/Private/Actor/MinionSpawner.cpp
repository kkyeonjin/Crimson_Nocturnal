#include "Actor/MinionSpawner.h"
#include "Character/Enemy/Enemy.h"

AMinionSpawner::AMinionSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMinionSpawner::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < MaxMinions; i++)
	{
		SpawnPointsIndex++;
		SpawnNewMinion();
	}

	SpawnPointsIndex = 0;
}

void AMinionSpawner::SpawnNewMinion()
{
	if (GetWorld())
	{
		AEnemy* NewMinion = GetWorld()->SpawnActor<AEnemy>(
			Monster,
			SpawnPoints[SpawnPointsIndex],
			FRotator::ZeroRotator);

		if (NewMinion)
		{
			NewMinion->SetOwner(this);

			AliveMinions.Add(NewMinion);
		}
	}
}

void AMinionSpawner::OnMinionDefeated(AActor* Minion)
{
	AEnemy* DefeatedEnemy = Cast<AEnemy>(Minion);

	if (DefeatedEnemy)
	{
		AliveMinions.Remove(DefeatedEnemy);  
	}

	SpawnPointsIndex = FMath::Rand() % 3;

	// Spawn new minion when a minion is dead
	// Maintain same minion numbers in fight zone
	GetWorldTimerManager().SetTimer(
		RespawnTimer,
		this,
		&AMinionSpawner::SpawnNewMinion,
		5.f,  
		false    
	);
}

// Called when Boss UltAttack Starts for clear fight zone
void AMinionSpawner::ClearAllMinions()
{
	for (int32 i = AliveMinions.Num() - 1; i >= 0; --i)
	{
		if (AliveMinions[i])
		{
			AliveMinions[i]->Destroy();
		}
	}
	AliveMinions.Empty();
}

