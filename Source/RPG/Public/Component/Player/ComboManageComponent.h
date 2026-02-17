// Manages combo input sequences, chain attacks, and boss QTE
// Broadcasts to PlayerCharacter for animation/BP rewards

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Player/Types/ComboInput.h"
#include "ComboManageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComboMatch, int32, ComboNum, int32, BPValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChainAttack, int32, AttackAnimNum, FName, SectionName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTEFailed);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UComboManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UComboManageComponent();

	// Check Combo Attack is complete or not
	bool IsComboMatch(EAttackKeyType Key);

	// Increment chain attack counter for basic attacks
	void ChainAttackCount(EAttackKeyType Key);

	void StartQTETimer();

	//Check BossUlt QTE Matched 
	bool BossUltimateQTE(EAttackKeyType Key);

	// Delegates (external binding from PlayerCharacter)
	UPROPERTY()
	FOnComboMatch OnComboMatch;

	UPROPERTY()
	FOnChainAttack OnChainAttack;

	UPROPERTY()
	FOnQTEFailed OnQTEFailed;

protected:

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

	bool ProcessComboInput(EAttackKeyType Key, bool bUseAllPatterns);


	int32 RequiredBossComboNum = 0;

public:
	FORCEINLINE float GetCurrentAttackMultiplier() { return CurrentAttackMultiplier; }
	// Get combo patterns for UI / Skill system
	FORCEINLINE TArray<FComboPattern>* GetComboPatterns() { return &ComboPatterns; }
	// Set random boss QTE target combo
	FORCEINLINE void SetRequiredBossComboNum() { RequiredBossComboNum = FMath::Rand() % 4; }
	FORCEINLINE int32 GetRequiredBossComboNum() { return RequiredBossComboNum; }
	FORCEINLINE int32 GetComboAnimMontage(int32 ComboAnimNum) { return ComboPatterns[ComboAnimNum].AnimMontageNum; }
};