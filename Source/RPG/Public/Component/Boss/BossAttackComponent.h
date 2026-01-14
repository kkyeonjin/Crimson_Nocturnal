// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component/Enemy/EnemyAttackComponent.h"
#include "BossAttackComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UBossAttackComponent : public UEnemyAttackComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBossAttackComponent();

	virtual void OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:


};
