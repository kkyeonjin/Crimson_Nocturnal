// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Character/Enemy/Types/EnemyType.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Interface/DefeatInterface.h"
#include "Interface/InfectableInterface.h"
#include "Enemy.generated.h"

class UInfectionComponent;
class UBehaviorTreeComponent;
class UEnemyAttackComponent;
class UEnemyAnimInstance;
class UEnemyAttributeComponent;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyAttackEnd);

UCLASS()
class RPG_API AEnemy : public ACharacter, public IHitInterface, public IDefeatInterface, public IInfectableInterface
{
	GENERATED_BODY()

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint, AActor* unused) override;

	// Sets default values for this character's properties
	AEnemy();


	
	UFUNCTION()
	void AttackStart(UBehaviorTreeComponent* BTComp, UBTTaskNode* TaskNode);

	UFUNCTION()
	void OnEnemyAttackMontageEnded();

	UFUNCTION()
	void ReadyToInfect();

	FOnEnemyAttackEnd OnEnemyAttackEnd;

	FOnCharacterDefeated OnEnemyDefeated;

	UFUNCTION()
	void OnInfectionStarted();

	UFUNCTION()
	void OnInfectionEnded();

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


	
	//쓰는지 안 쓰는지 확인 필
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float PatrolSpeed = EnemyConstants::PATROL_SPEED;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float ChaseSpeed = EnemyConstants::CHASE_SPEED;
	

	
private:
	//for safe referencing
	bool InitializeComponents();

	void BindDelegates();

	UEnemyAnimInstance* CachedEnemyAnimInstance;
	UEnemyAttributeComponent* CachedEnemyAttributeComponent;
	UEnemyAttackComponent* CachedEnemyAttackComponent;
	UInfectionComponent* CachedInfectionComponent;

	bool bReadyToInfect = false;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* HitParticles;

	//Materials
	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> OriginalMaterials;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UFUNCTION()
	void StartDeathSequence();

	UFUNCTION()
	void FadeOutAndDestroy();

	void InitializeMaterials();


	//tracking Current Materials for Material change/restore,,,,etc
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CurrentAppliedMaterials;

	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, Category = "Death")
	float DeathAnimationDuration = EnemyConstants::DEATH_ANIMATION_DURATION; 

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float FadeOutDuration = EnemyConstants::FADEOUT_DURATION; 

	float FadeOutAlpha = 1.0f;

	//UMaterialInterface* OriginalMaterial;

	//각 Enemy마다 BT, BB 가지고 관리하도록 변수 선언 
	UBehaviorTreeComponent* CachedBTComponent = nullptr;
	UBTTaskNode* CachedBTTaskNode = nullptr;

	EEnemyCombatState EnemyCombatState = EEnemyCombatState::EECS_NotAttacking;

	//IDefeatable
	virtual FOnCharacterDefeated& OnCharacterDefeatedDelegate() override;

	//IInfectable
	virtual USkeletalMeshComponent* GetMeshComponent_Implementation() override;
	virtual TArray<UMaterialInstanceDynamic*> GetOriginalDynamicMaterials_Implementation() override;
	virtual void SetCurrentMaterial_Implementation(UMaterialInstanceDynamic* Material) override;

public:
	//FORCEINLINE bool IsDead() { return bIsDead; }
	FORCEINLINE EEnemyCombatState GetEnemyCombatState() { return EnemyCombatState; }
	FORCEINLINE void SetEnemyCombatState(EEnemyCombatState State) { EnemyCombatState = State; }
	FORCEINLINE FName GetEnemyTypeName() { return EnemyTypeName; }
	
};
