// Spawning particle for Boss Phase3 Attack (Ground Slam)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroundSlam.generated.h"

class UEnemyAttackComponent;

UCLASS()
class RPG_API AGroundSlam : public AActor
{
	GENERATED_BODY()
	
public:
	AGroundSlam();

	void StartSlam();

	void CollisionEnable();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> LighteningActor;


private:

	UPROPERTY(EditAnywhere)
	TArray<FVector> SpawnPoints;

	TArray<AActor*> SpawnedLightningActors;

	UEnemyAttackComponent* CachedAttackComponent;

	FTimerHandle CollisionStartTimer;

};
