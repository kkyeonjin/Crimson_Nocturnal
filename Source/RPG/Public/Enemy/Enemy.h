// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UEnemyAttackComponent;
class UEnemyAnimInstance;
class UEnemyAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyAttackEnd);

UCLASS()
class RPG_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint, AActor* unused) override;

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

	UFUNCTION()
	void SetAttackCollisionEnable();

	UFUNCTION()
	void SetAttackCollisionDisable();

	UFUNCTION(BlueprintCallable)
	void SetEnemyGroundSpeed(float NewSpeed);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float PatrolSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float ChaseSpeed = 500.f;

private:
	UEnemyAnimInstance* CachedEnemyAnimInstance;
	UEnemyAttributeComponent* CachedEnemyAttributeComponent;
	UEnemyAttackComponent* CachedEnemyAttackComponent;

	bool IsAttacking;

	bool bIsDead;

public:
	FORCEINLINE bool IsDead() { return bIsDead; }
	
};
