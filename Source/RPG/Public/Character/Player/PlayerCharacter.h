/**
 * @class APlayerCharacter
 * @brief Core Player Character class implementing component-based architecture
 * Routes input to specialized components via delegates/events instead of direct references (loose coupling)
 * Manages combat/movement states and orchestrates combo/QTE/ultimate skill flow between components
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Character/Player/Types/SkillTypes.h"
#include "Interface/HitInterface.h"
#include "GameFramework/PlayerController.h"
#include "Character/Player/Types/PlayerTypes.h"
#include "Character/Player/Types/ComboInput.h"
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
class UInteractionComponent;
class USFXComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillCReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTESuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractPressed);

UCLASS()
class RPG_API APlayerCharacter : public ACharacter, public IHitInterface //, public IDefeatInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleAttack(AActor* Target);

	// Apply damage when player gets hit via HitInterface
	void GetHit(const FVector& ImpactPoint, AActor* Target) override;

	// Start boss QTE mode: lock player movement and show QTE UI
	void StartBossQTEMode();

	
	// PopUp Functions 
	UFUNCTION()
	void InteractPopUp();
	void ShowStartPopUp();
	void DancePopUp();
	void HideInteractPopUp();


	// Public delegates for inter-component communication
	FOnSkillCReady OnSkillCReady;
	FOnQTESuccess OnQTESuccess;
	FOnInteractPressed OnInteractPressed;


protected:
	virtual void BeginPlay() override;


	//Input assets
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


	// UI open/close actions
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SkillTreeOn;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* PopUpOff;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MenuOn;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Interact;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* HideDialogue;


	// Saloon event-specific context (map-based priority adjustment)
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SaloonContext;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Dance;


	//Input handlers
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Handle player basic attack input (X/Y) and route to combo/QTE/ultimate
	// Applies attack state & disables movement during attack
	void Attack(const FInputActionValue& Value);
	// Attack handlers with InputValue
	void HandleUltimateSkillAttack(float AttackValue);
	EAttackKeyType GetAttackKeyType(float AttackValue);
	void HandleQTEAttack(EAttackKeyType InputType, float AttackValue);
	void HandleNormalComboAttack(EAttackKeyType InputType);

	void SkillStart();
	void OnInteract();
	//Open SKillTree
	void OpenSkillTree();
	//Saloon event
	void StartDance();
	virtual void Jump() override;


	//Animation/Combo callbacks
	UFUNCTION()
	void OnAttackMontageEnded();

	UFUNCTION()
	void OnComboComplete(int32 ComboNum, int32 BPValue);

	UFUNCTION()
	void OnChainAttack(int32 AnimNum, FName SectionName);

	UFUNCTION()
	void SetMoveEnable();

	// Weapon/kick collision enable/disable callbacks (called from animation notifies)
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


	//UI/Attribute callbacks
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
	void OnSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index);
	UFUNCTION()
	void OnSkillTreeSet(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills, int32 Ult1XSetting, int32 Ult1YSetting);

	UFUNCTION()
	void SetIsSkillEnable();
	UFUNCTION()
	void UltTimeOver();
	UFUNCTION()
	void OnQTEFailed();
	UFUNCTION()
	void OnUltSkillAAttack(int32 AnimNum, float AttackMultiplier);
	UFUNCTION()
	void OpenGameOverLevel();


private:
	/** Initialization split for performance + readability */
	void SetupInputSubsystem();
	void CacheComponents();
	void BindDelegates();
	UFUNCTION()
	void InitializeHUD();


	// Cached subsystems/components
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;


	// Player states
	EPlayerCombatState PlayerCombatState = EPlayerCombatState::EPCS_NotAttacking;
	EPlayerMoveState PlayerMoveState = EPlayerMoveState::EPMS_MoveEnabled;
	EUltimateSkill SelectedSkill = EUltimateSkill::None;


	// Cached components for performance
	UPlayerAnimInstance* CachedAnimInstance;
	UComboManageComponent* CachedComboManageComponent;
	UWeaponComponent* CachedWeaponComponent;
	UKickComponent* CachedKickComponent;
	UPlayerAttributeComponent* CachedAttributeComponent;
	UUltimateSkillComponent* CachedUltimateSkillComponent;
	UHUDComponent* CachedHUDComponent;
	UInteractionComponent* CachedInteractionComponent;
	USFXComponent* CachedSFXComponent;


	// State flags
	bool IsSkillOn = false;
	bool IsTrapped = false;
	//turn true when BP is full
	bool IsSkillEnable = false;


public:
	// INLINE getters & setters
	FORCEINLINE EPlayerCombatState GetPlayerCombatState() const { return PlayerCombatState; }
	FORCEINLINE void SetPlayerCombatState(EPlayerCombatState State) { PlayerCombatState = State; }
	FORCEINLINE EPlayerMoveState GetPlayerMoveState() const { return PlayerMoveState; }
	FORCEINLINE void SetPlayerMoveState(EPlayerMoveState State) { PlayerMoveState = State; }
	FORCEINLINE void SetUltimateSkill(int32 SkillNum) { SelectedSkill = static_cast<EUltimateSkill>(SkillNum); }
	FORCEINLINE void SetIsSkillOn(bool bIsSkillOn) { IsSkillOn = bIsSkillOn; }
};

