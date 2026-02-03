// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Character/Player/Types/SkillTypes.h"
#include "Interface/HitInterface.h"
#include "GameFramework/PlayerController.h"
#include "Character/Player/Types/PlayerTypes.h" //for EPlayerState
#include "Character/Player/Types/ComboInput.h"
#include "Interface/InteractableInterface.h"
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
class URaycastComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillCReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTESuccess);

UCLASS()
class RPG_API APlayerCharacter : public ACharacter, public IHitInterface //, public IDefeatInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleAttack(AActor* Target);

	//HitInterface
	void GetHit(const FVector& ImpactPoint, AActor* Target) override;

	void StartBossQTEMode();

	FOnSkillCReady OnSkillCReady;

	FOnQTESuccess OnQTESuccess;

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* PlayerContext;

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

	//Open SkillTree Screen
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SkillTreeOn;

	//Open Menu Screen
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MenuOn;

	//Interact with Portal / NPC ...etc
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Interact;

	//Function for Input Binding 
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void SkillStart();

	//Menu
	//void MenuScreen();

	//Interact (Portal / NPC)
	void OnInteract();

	UFUNCTION()
	void SetIsSkillEnable();

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
	void OnLevelUp(int32 CurrentLevel, int32 SkillPoint);

	UFUNCTION()
	void OnDamageCalculated(int32 Damage);

	UFUNCTION()
	void OnSkillPointUpdate(int32 SkillPoint);

	UFUNCTION()
	void UltTimeOver();

	//UltA
	UFUNCTION()
	void OnSkill1Attack(int32 AnimNum, float AttackMultiplier);

	//Portal / NPC Detected (Raycast)
	UFUNCTION()
	void InteractPopUp();

	//Move to GameOver Level
	UFUNCTION()
	void OpenGameOverLevel();

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

	URaycastComponent* CachedRaycastComponent;

	bool IsSkillOn = false;

	bool IsTrapped = false;

	//turn true when BP is full
	bool IsSkillEnable = false;

	UFUNCTION()
	void OnSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index);

public:
	//FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE EPlayerCombatState GetPlayerCombatState() const { return PlayerCombatState; }
	FORCEINLINE void SetPlayerCombatState(EPlayerCombatState State) { PlayerCombatState = State; }

	FORCEINLINE EPlayerMoveState GetPlayerMoveState() const { return PlayerMoveState; }
	FORCEINLINE void SetPlayerMoveState(EPlayerMoveState State) { PlayerMoveState = State; }

	FORCEINLINE void SetUltimateSkill(int32 SkillNum) { SelectedSkill = static_cast<EUltimateSkill>(SkillNum + 1); }

	FORCEINLINE void SetIsSkillOn(bool bIsSkillOn) { IsSkillOn = bIsSkillOn; }

	//FORCEINLINE void SetIsTrapped(bool bIsTrapped) { IsTrapped = bIsTrapped; }
};

