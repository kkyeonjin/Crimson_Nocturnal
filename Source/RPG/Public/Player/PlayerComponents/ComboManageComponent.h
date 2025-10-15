// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Types/ComboInput.h"
#include "ComboManageComponent.generated.h"

//delegate rings(broadcast) when combo matches
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComboMatch, int32, ComboAnimNum, int32, BPValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChainAttack, int32, AttackAnimNum, FName, SectionName);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UComboManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComboManageComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsComboMatch(EAttackKeyType Key);

	void ChainAttackCount(EAttackKeyType Key);

	UPROPERTY()
	FOnComboMatch OnComboMatch;

	UPROPERTY()
	FOnChainAttack OnChainAttack;

	int32 GetComboAnimMontage(int32 ComboAnimNum);



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	TArray<FComboPattern> ComboPatterns;

private:
	TArray<EAttackKeyType> InputKeySequence;

	UPROPERTY(EditAnywhere, Category = "Combo")
	float ComboTimeOut = 2.f;

	FTimerHandle ComboTimeOutTimer;

	float TimeSinceLastInput = 0.f;

	int32 ChainAttackCountX = 0;

	int32 ChainAttackCountY = 0;

	float CurrentAttackMultiplier;

	void ResetInputSequence();

	void InitializeComboPattern();

	void CheckComboTimeOut();

public:
	FORCEINLINE float GetCurrentChainAttackMultiplier() { return CurrentAttackMultiplier; }


};
