// Start battle func & logics when player get in the boss fight zone
// manage boss fight zone collision

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossBattleArena.generated.h"

class ABossDoor;
class AMinionSpawner;
class ABoss;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleTriggered);

UCLASS()
class RPG_API ABossBattleArena : public AActor
{
	GENERATED_BODY()

public:
	ABossBattleArena();

	UPROPERTY(EditAnywhere)
	UBoxComponent* BattleTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena")
	ABossDoor* ArenaDoor;  

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

	// func calling interval when DoorTimer start
	float DoorCloseTime = 0.016f;
	float DoorAnimationElapsedTime = 0.f;
	float DoorAnimationDuration = 2.f;

	FTimerHandle MinionSpawnerTimer;
	AMinionSpawner* CachedMinionSpawner;

};
