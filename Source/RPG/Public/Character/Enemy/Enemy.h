/**
 * @class AEnemy
 * @brief Generic enemy with AI, infection, attack animations via components
 * BT-driven attacks, delegate-based anim/attribute sync, fade-out death
 * Supports SkillC infection spreading from player combos
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Character/Enemy/Types/EnemyType.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Interface/InfectableInterface.h"
#include "Enemy.generated.h"

class UInfectionComponent;
class UBehaviorTreeComponent;
class UEnemyAttackComponent;
class UEnemyAnimInstance;
class UEnemyAttributeComponent;
class APlayerCharacter;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyAttackEnd);

UCLASS()
class RPG_API AEnemy : public ACharacter, public IHitInterface, public IInfectableInterface
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** HitInterface: damage + infection + death sequence */
	virtual void GetHit(const FVector& ImpactPoint, AActor* unused) override;

	AEnemy();


	// SFX assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX")
	USoundCue* AttackSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX")
	USoundCue* HitSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX")
	USoundCue* DeathSFX;

	
	UFUNCTION()
	void AttackStart(UBehaviorTreeComponent* BTComp, UBTTaskNode* TaskNode);

	UFUNCTION()
	void OnEnemyAttackMontageEnded();

	//UltSkillC infection trigger
	UFUNCTION()
	void ReadyToInfect();
	UFUNCTION()
	void OnInfectionStarted();
	UFUNCTION()
	void OnInfectionEnded();

	// delegate
	FOnEnemyAttackEnd OnEnemyAttackEnd;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetAttackCollisionEnable();

	UFUNCTION()
	void SetAttackCollisionDisable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FName EnemyTypeName = FName(TEXT("Mutant"));
	
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

	// Materials for infection visuals
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

	// to manage AI state at each enemy (prevent to move same compared to other enemies)
	UBehaviorTreeComponent* CachedBTComponent = nullptr;
	UBTTaskNode* CachedBTTaskNode = nullptr;

	EEnemyCombatState EnemyCombatState = EEnemyCombatState::EECS_NotAttacking;

	//IInfectable
	virtual USkeletalMeshComponent* GetMeshComponent_Implementation() override;
	virtual TArray<UMaterialInstanceDynamic*> GetOriginalDynamicMaterials_Implementation() override;
	virtual void SetCurrentMaterial_Implementation(UMaterialInstanceDynamic* Material) override;


public:
	FORCEINLINE EEnemyCombatState GetEnemyCombatState() { return EnemyCombatState; }
	FORCEINLINE void SetEnemyCombatState(EEnemyCombatState State) { EnemyCombatState = State; }
	FORCEINLINE FName GetEnemyTypeName() { return EnemyTypeName; }
	
};
