/**
 * @class ABoss
 * @brief Boss AI managing phases, crystals, ultimate QTE via components
 * Delegates handle phase transitions, infection, HP thresholds (loose coupling)
 * Tracks crystals destruction ¡æ final phase trigger
 */

#pragma once

#include "CoreMinimal.h"
#include "Character/Boss/Types/BossTypes.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Interface/InfectableInterface.h"
#include "Boss.generated.h"

class UEnemyAttackComponent;
class UBossAttackComponent;
class UInfectionComponent;
class ACrystal;
class ABossUltimateSkill;
class APlayerCharacter;
class UBossAnimInstance;
class UBossAttributeComponent;
class UPhaseManageComponent;
class UBossSFXComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossBattleStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLastAttackStarted);

UCLASS()
class RPG_API ABoss : public ACharacter, public IHitInterface, public IInfectableInterface
{
	GENERATED_BODY()

public:
	ABoss();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** HitInterface: applies damage + infection check + phase change */
	virtual void GetHit(const FVector& ImpactPoint, AActor* Target) override;

	UFUNCTION()
	void ReadyToInfect();

	UFUNCTION()
	void OnInfectionStarted();

	UFUNCTION()
	void OnInfectionEnded();

	UFUNCTION()
	void StartBattle(); //Called when BattleStart Anim is Done (Notify)

	UFUNCTION()
	void OnPhaseChange(EBossPhase NewPhase);

	UFUNCTION()
	void OnBattleTriggered();

	UFUNCTION()
	void OnCollisionEnable();

	UFUNCTION()
	void OnCollisionDisable();

	UFUNCTION()
	void OnHpThreshold(int32 ThresholdStep);

	UFUNCTION()
	void OnPlayerQTESuccess();

	UFUNCTION()
	void OnLastCrystalDestroyed();

	UFUNCTION()
	void StartEnding();


	// Public delegates
	UPROPERTY()
	FOnBossBattleStarted OnBossBattleStarted;
	UPROPERTY()
	FOnLastAttackStarted OnLastAttackStarted;


	// Triggers QTE mechanic
	void StartUltimateMechanic();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

private:
	// Component caching & validation
	bool InitializeComponents();
	void BindDelegates();


	// components for performance
	UBossAnimInstance* CachedBossAnimInstance;
	UPhaseManageComponent* CachedPhaseManageComponent;
	UBossAttributeComponent* CachedBossAttributeComponent;
	UEnemyAttackComponent* CachedAttackComponent;
	UInfectionComponent* CachedInfectionComponent;
	UBossSFXComponent* CachedBossSFXComponent;

	EBossPhase CurrentPhase;
	bool bBattleActivated = false;
	bool bReadyToInfect = false;


	// Dynamic materials for infection visual effects
	void InitializeMaterials();

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> OriginalMaterials;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	//tracking Current Materials for Material change/restore,,,,etc
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CurrentAppliedMaterials;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitParticles;


	//UltSkill spawning
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABossUltimateSkill> UltSkillClass;
	ABossUltimateSkill* UltActor;
	FTimerHandle UltSkillActorTimerHandle;
	void SpawnUltimateActor();

	UPROPERTY(EditAnywhere)
	FVector UltSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Crystals")
	TArray<ACrystal*> Crystals;  


	//IInfectable
	virtual USkeletalMeshComponent* GetMeshComponent_Implementation() override;
	virtual TArray<UMaterialInstanceDynamic*> GetOriginalDynamicMaterials_Implementation() override;
	virtual void SetCurrentMaterial_Implementation(UMaterialInstanceDynamic* Material) override;

	bool IsUltSkillStarted = false;


public:
	FORCEINLINE EBossPhase GetCurrentPhase() { return CurrentPhase; }
};
