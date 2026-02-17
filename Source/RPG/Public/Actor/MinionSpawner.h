// Spawn minions(monster) in boss fight zone

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinionSpawner.generated.h"

class AEnemy;

UCLASS()
class RPG_API AMinionSpawner : public AActor
{
	GENERATED_BODY()

public:
	AMinionSpawner();

	void SpawnNewMinion();

	UFUNCTION()
	void OnMinionDefeated(AActor* Minion);

	UFUNCTION()
	void ClearAllMinions();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy> Monster;

private:
	int32 MaxMinions = 3;

	UPROPERTY(EditAnywhere)
	TArray<FVector> SpawnPoints;

	int32 SpawnPointsIndex = -1;

	UPROPERTY()
	TArray<AEnemy*> AliveMinions;  
	FTimerHandle RespawnTimer;

};
