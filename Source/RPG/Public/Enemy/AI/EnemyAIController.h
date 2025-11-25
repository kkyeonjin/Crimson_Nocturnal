// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

struct FAIStimulus;
class AEnemy;

//class UAIPerceptionComponent;

/**
 * 
 */
UCLASS()
class RPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
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
