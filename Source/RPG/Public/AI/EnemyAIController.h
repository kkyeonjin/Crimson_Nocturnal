// For TargetActor setting (basic actions in enemy AI)

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

struct FAIStimulus;
class AEnemy;

UCLASS()
class RPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	float AttackRadius;

	//Player actor
	AActor* TargetActor;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	AEnemy* Owner;
};
