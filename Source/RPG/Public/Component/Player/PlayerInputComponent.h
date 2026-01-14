// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInputComponent.generated.h"


class UPlayerAttributeComponent;
class UComboManageComponent;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UPlayerInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInputComponent();

	/*
	//All Callback Functions in PlayerCharacter
	UFUNCTION()
	void OnAttackMontageEnded();

	UFUNCTION()
	void OnComboComplete(int32 ComboNum, int32 BPValue);

	UFUNCTION()
	void OnChainAttack(int32 AnimNum, FName SectionName);

	UFUNCTION()
	void SetMoveEnable();

	UFUNCTION()
	void SetBladeLCollisionEnable();

	UFUNCTION()
	void SetBladeLCollisionDisable();

	UFUNCTION()
	void SetBladeRCollisionEnable();

	UFUNCTION()
	void SetBladeRCollisionDisable();

	UFUNCTION()
	void SetKickCollisionEnable();

	UFUNCTION()
	void SetKickCollisionDisable();

	UFUNCTION()
	void UpdateBP(int32 CurrentBP, int32 MaxBP);

	UFUNCTION()
	void UpdateHP(int32 CurrentHP, int32 MaxHP);

	UFUNCTION()
	void OnSkillTreeUISet(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills);

	UFUNCTION()
	void OnLevelUp(int32 SkillPoint);

	UFUNCTION()
	void OnDamageCalculated(int32 Damage);

	UFUNCTION()
	void OnSkillPointUpdate(int32 SkillPoint);

	//SkillA
	UFUNCTION()
	void OnSkill1Attack(int32 AnimNum, float AttackMultiplier);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

private:
	UPROPERTY()
	APlayerCharacter* OwnerPlayer;

	// 필요한 컴포넌트 참조들만
	UPROPERTY()
	UComboManageComponent* CachedComboComponent;

	UPROPERTY()
	UPlayerAttributeComponent* CachedAttributeComponent;

	// ... 필요한 것들만

	void BindDelegates();
	*/
};
