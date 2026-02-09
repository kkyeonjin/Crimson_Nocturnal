// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	AGroundSlam();

	void StartSlam();

	void CollisionEnable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> LighteningActor;



private:

	UPROPERTY(EditAnywhere)
	TArray<FVector> SpawnPoints;

	TArray<AActor*> SpawnedLightningActors;

	UEnemyAttackComponent* CachedAttackComponent;

	FTimerHandle CollisionStartTimer;

	//float CollisionStartTime;

};
