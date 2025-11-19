// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy/Types/EnemyType.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Enemy.generated.h"

class UBehaviorTreeComponent;
class UEnemyAttackComponent;
class UEnemyAnimInstance;
class UEnemyAttributeComponent;
class APlayerCharacter;

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

	void RestoreMaterialToDefault();

	UFUNCTION()
	void AttackStart(UBehaviorTreeComponent* BTComp, UBTTaskNode* TaskNode);

	UFUNCTION()
	void OnEnemyAttackMontageEnded();

	UFUNCTION()
	void ReadyToInfect();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FName EnemyTypeName = FName(TEXT("Mutant"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float PatrolSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float ChaseSpeed = 500.f;

private:
	UEnemyAnimInstance* CachedEnemyAnimInstance;
	UEnemyAttributeComponent* CachedEnemyAttributeComponent;
	UEnemyAttackComponent* CachedEnemyAttackComponent;

	bool bReadyToInfect = false;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Material")
	UMaterialInterface* RedMaterial;

	UMaterialInterface* OriginalMaterial;

	// BTTask 관련 캐싱 변수 추가
	UBehaviorTreeComponent* CachedBTComponent = nullptr;
	UBTTaskNode* CachedBTTaskNode = nullptr;

	EEnemyCombatState EnemyCombatState = EEnemyCombatState::EECS_NotAttacking;

public:
	//FORCEINLINE bool IsDead() { return bIsDead; }
	FORCEINLINE EEnemyCombatState GetEnemyCombatState() { return EnemyCombatState; }
	FORCEINLINE void SetEnemyCombatState(EEnemyCombatState State) { EnemyCombatState = State; }
	FORCEINLINE FName GetEnemyTypeName() { return EnemyTypeName; }

};
