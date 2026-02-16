// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Boss/Boss.h"
#include "Character/Boss/Types/BossTypes.h"
#include "PhaseManageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChange, EBossPhase, CurrentPhase);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UPhaseManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPhaseManageComponent();

	EBossPhase SelectRandomPhase();

	void ExecuteCurrentPhase();

	UPROPERTY()
	FOnPhaseChange OnPhaseChange;

	void Phase1();

	//Defined with UFUNCTION cause this it called by AnimNotify & Delegate Callback
	UFUNCTION()
	void Phase3();

	void UltimateAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	EBossPhase CurrentPhase = EBossPhase::Phase1_MagicCircle;

	//void Phase1();
	void Phase2();
	//void Phase3();

	float Phase1Duration = BossConstants::PHASE1_DURATION;
	float Phase2Duration = BossConstants::PHASE2_DURATION;
	float Phase3Duration = BossConstants::PHASE3_DURATION;

	FTimerHandle PhaseTimer;

	void StartPhaseTimer();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MagicCircleAoEClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> WingAttackClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GroundSlamClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> UltClass;

	UPROPERTY(EditAnywhere)
	FVector Center;

	TArray<EBossPhase> AvailablePhases;

	bool bPreventConsecutivePhases = true;

	bool bIsPhaseExecutionEnable = true;

public:
	FORCEINLINE EBossPhase GetCurrentPhase() { return CurrentPhase; }
	FORCEINLINE void StopPhaseExecution() { bIsPhaseExecutionEnable = false; }

};
