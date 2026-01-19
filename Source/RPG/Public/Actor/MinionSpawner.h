// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	AMinionSpawner();

	void SpawnNewMinion();

	UFUNCTION()
	void OnMinionDefeated(AActor* Minion);

	UFUNCTION()
	void ClearAllMinions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy> Monster;

private:
	int32 MaxMinions = 2;

	UPROPERTY(EditAnywhere)
	TArray<FVector> SpawnPoints;

	int32 SpawnPointsIndex = -1;

	UPROPERTY()
	TArray<AEnemy*> AliveMinions;  // TWeakObjectPtr<AEnemy> Á¦°Å

	FTimerHandle RespawnTimer;

public:

};
