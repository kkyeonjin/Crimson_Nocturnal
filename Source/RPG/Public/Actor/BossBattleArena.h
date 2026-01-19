// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossBattleArena.generated.h"

class AMinionSpawner;
class ABoss;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleTriggered);

UCLASS()
class RPG_API ABossBattleArena : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossBattleArena();

	UPROPERTY(EditAnywhere)
	UBoxComponent* BattleTrigger;

	UPROPERTY(EditAnywhere)
	AActor* ArenaDoor;

	UPROPERTY(EditAnywhere)
	ABoss* BossActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MinionSpawnerClass;

	void StartBattle();

	UFUNCTION(BlueprintCallable)
	void InitializeCollisionBox(UBoxComponent* CollisionBox);

	UFUNCTION()
	void ClearMinionSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBattleTriggerOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY()
	FOnBattleTriggered OnBattleTriggered;

	void SpawnMinionSpawner();

private:
	bool bBattleActivated = false;

	UPROPERTY(EditAnywhere)
	FVector ClosedDoorPosition;

	void CloseDoor();
	FTimerHandle DoorTimer;

	FTimerHandle MinionSpawnerTimer;

	float DoorCloseTime = 0.016f;
	float DoorAnimationElapsedTime = 0.f;
	float DoorAnimationDuration = 2.f;

	AMinionSpawner* CachedMinionSpawner;


public:


};
