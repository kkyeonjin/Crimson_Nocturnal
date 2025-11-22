// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/HitInterface.h"
#include "GameFramework/PlayerController.h"
#include "Player/Types/PlayerType.h" //for EPlayerState
#include "Player/Types/ComboInput.h"
#include "PlayerCharacter.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UHUDComponent;
class UUltimateSkillComponent;
class UPlayerAttributeComponent;
class UKickComponent;
class UWeaponComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class UPlayerAnimInstance;
class UComboManageComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillCReady);

UCLASS()
class RPG_API APlayerCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleAttack(AActor* Target);

	void GetHit(const FVector& ImpactPoint, AActor* Target) override;

	FOnSkillCReady OnSkillCReady;

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SkillTreeContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SkillActivation;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SkillTreeOn;

	//SkillTreeContext 
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SelectAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SkillTreeOffAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SkillEquipAction;

	//Function for Input Binding 
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void SkillStart();
	void SkillTree();

	//SkillTreeInputContext
	void Select(const FInputActionValue& Value);
	void SkillTreeOff();
	void SetUlt();

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
	void UpdateBP();

	UFUNCTION()
	void UpdateHP();

	UFUNCTION()
	void OnSkillTreeUISet(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills);

	UFUNCTION()
	void OnLevelUp(int32 SkillPoint);

	UFUNCTION()
	void OnDamageCalculated(int32 Damage);

	//SkillA
	UFUNCTION()
	void OnSkill1Attack(int32 AnimNum, float AttackMultiplier);

private:

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;

	//default state is unequipped
	EPlayerCombatState PlayerCombatState = EPlayerCombatState::EPCS_NotAttacking;

	//default state is moveEnabled
	EPlayerMoveState PlayerMoveState = EPlayerMoveState::EPMS_MoveEnabled;

	EUltimateSkill SelectedSkill = EUltimateSkill::SkillA;

	UPlayerAnimInstance* CachedAnimInstance;

	UComboManageComponent* CachedComboManageComponent;

	UWeaponComponent* CachedWeaponComponent;

	UKickComponent* CachedKickComponent;

	UPlayerAttributeComponent* CachedAttributeComponent;

	UUltimateSkillComponent* CachedUltimateSkillComponent;

	UHUDComponent* CachedHUDComponent;

	bool IsSkillOn = false;

	//only visible in Detail Panel
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE EPlayerCombatState GetPlayerCombatState() const { return PlayerCombatState; }
	FORCEINLINE void SetPlayerCombatState(EPlayerCombatState State) { PlayerCombatState = State; }

	FORCEINLINE EPlayerMoveState GetPlayerMoveState() const { return PlayerMoveState; }
	FORCEINLINE void SetPlayerMoveState(EPlayerMoveState State) { PlayerMoveState = State; }

	FORCEINLINE void SetUltimateSkill(int32 SkillNum) { SelectedSkill = static_cast<EUltimateSkill>(SkillNum + 1); }

	FORCEINLINE void SetIsSkillOn(bool bIsSkillOn) { IsSkillOn = bIsSkillOn; }
};
