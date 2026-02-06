// Fill out your copyright notice in the Description page of Project Settings.

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
class ABossUltimateSkillActor;
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
	// Sets default values for this character's properties
	ABoss();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint, AActor* Target) override;

	UFUNCTION()
	void ReadyToInfect();

	UFUNCTION()
	void OnInfectionStarted();

	UFUNCTION()
	void OnInfectionEnded();

	UFUNCTION()
	void StartBattle(); //Called when BattleStart Anim is Done (Notify)

	void EndBattle();

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

	UPROPERTY()
	FOnBossBattleStarted OnBossBattleStarted;

	UPROPERTY()
	FOnLastAttackStarted OnLastAttackStarted;

	void StartUltimateMechanic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

private:

	bool InitializeComponents();

	void BindDelegates();

	UBossAnimInstance* CachedBossAnimInstance;
	UPhaseManageComponent* CachedPhaseManageComponent;
	UBossAttributeComponent* CachedBossAttributeComponent;
	UEnemyAttackComponent* CachedAttackComponent;
	UInfectionComponent* CachedInfectionComponent;
	UBossSFXComponent* CachedBossSFXComponent;

	EBossPhase CurrentPhase;

	bool bBattleActivated = false;

	bool bReadyToInfect = false;

	void InitializeMaterials();

	//Materials
	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> OriginalMaterials;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	//tracking Current Materials for Material change/restore,,,,etc
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CurrentAppliedMaterials;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitParticles;

	//UltSkill,,,
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<ABossUltimateSkillActor> UltSkillActorClass;

	FTimerHandle RoarTimerHandle;     // 1.5초 타이머
	FTimerHandle UltSkillActorTimerHandle;

	//for debug
	FTimerHandle DebugTimer;

	void SpawnUltimateActor();

	UPROPERTY(EditAnywhere)
	FVector UltSpawnPoint;

	UPROPERTY(EditAnywhere)
	ACrystal* Crystal;

	//IInfectable
	virtual USkeletalMeshComponent* GetMeshComponent_Implementation() override;
	virtual TArray<UMaterialInstanceDynamic*> GetOriginalDynamicMaterials_Implementation() override;
	virtual void SetCurrentMaterial_Implementation(UMaterialInstanceDynamic* Material) override;

public:
	FORCEINLINE EBossPhase GetCurrentPhase() { return CurrentPhase; }


};
