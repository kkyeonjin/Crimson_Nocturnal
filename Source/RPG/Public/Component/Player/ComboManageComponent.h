// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Player/Types/ComboInput.h"
#include "ComboManageComponent.generated.h"


//delegate rings(broadcast) when combo matches
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComboMatch, int32, ComboNum, int32, BPValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChainAttack, int32, AttackAnimNum, FName, SectionName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTEFailed);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UComboManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComboManageComponent();

	//Check Combo Attack is complete or not
	bool IsComboMatch(EAttackKeyType Key);

	//check normal(chain) attack times 
	void ChainAttackCount(EAttackKeyType Key);

	UPROPERTY()
	FOnComboMatch OnComboMatch;

	UPROPERTY()
	FOnChainAttack OnChainAttack;

	UPROPERTY()
	FOnQTEFailed OnQTEFailed;

	int32 GetComboAnimMontage(int32 ComboAnimNum);

	//Check BossUlt QTE Matched (Renaming Required)
	bool BossUltimateQTE(EAttackKeyType Key);

	void StartQTETimer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	TArray<FComboPattern> ComboPatterns;

private:
	TArray<EAttackKeyType> InputKeySequence;

	UPROPERTY(EditAnywhere, Category = "Combo")
	float ComboTimeOut = ComboConstants::COMBO_TIMEOUT;

	FTimerHandle ComboTimeOutTimer;

	FTimerHandle BossSurvivalTimer;

	float BossSurvivalTimeOut = ComboConstants::BOSS_SURVIVAL_TIMEOUT;

	float TimeSinceLastInput = 0.f;

	int32 ChainAttackCountX = 0;

	int32 ChainAttackCountY = 0;

	float CurrentAttackMultiplier;

	void ResetInputSequence();

	void InitializeComboPattern();

	void CheckComboTimeOut();

	void QTEFailed();

	int32 RequiredBossComboNum = 0;

public:
	FORCEINLINE float GetCurrentAttackMultiplier() { return CurrentAttackMultiplier; }
	FORCEINLINE TArray<FComboPattern>* GetComboPatterns() { return &ComboPatterns; }

	FORCEINLINE void SetRequiredBossComboNum() { RequiredBossComboNum = 0;/*RequiredBossComboNum = FMath::Rand() % 4;*/ UE_LOG(LogTemp, Warning, TEXT("Combo Num: %d"), RequiredBossComboNum); }
	FORCEINLINE int32 GetRequiredBossComboNum() { return RequiredBossComboNum; }
};