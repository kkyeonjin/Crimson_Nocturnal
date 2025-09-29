// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UEnemyAnimInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyAttackEnd);

UCLASS()
class RPG_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sets default values for this character's properties
	AEnemy();

	UFUNCTION()
	void AttackStart();

	UFUNCTION()
	void OnEnemyAttackMontageEnded();

	FOnEnemyAttackEnd OnEnemyAttackEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetEnemyGroundSpeed(float NewSpeed);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float PatrolSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float ChaseSpeed = 500.f;

private:
	UEnemyAnimInstance* EnemyAnimInstance;

	bool IsAttacking;
	
};
