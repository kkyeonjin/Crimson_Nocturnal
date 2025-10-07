// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerType.h" //for EPlayerState
#include "ComboInput.h"
#include "PlayerCharacter.generated.h"

class UPlayerAttributeComponent;
class UKickComponent;
class UWeaponComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class UPlayerAnimInstance;
class UComboManageComponent;

UCLASS()
class RPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleAttack(AActor* Target);

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	//Function for Input Binding 
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	UFUNCTION()
	void OnAttackMontageEnded();

	UFUNCTION()
	void OnComboComplete(int32 ComboNum);

	UFUNCTION()
	void SetMoveEnable();

	UFUNCTION()
	void SetWeaponCollisionEnable();

	UFUNCTION()
	void SetWeaponCollisionDisable();

	UFUNCTION()
	void SetKickCollisionEnable();

	UFUNCTION()
	void SetKickCollisionDisable();

private:
	//default state is unequipped
	EPlayerCombatState PlayerCombatState = EPlayerCombatState::EPCS_NotAttacking;

	//default state is moveEnabled
	EPlayerMoveState PlayerMoveState = EPlayerMoveState::EPMS_MoveEnabled;

	UPlayerAnimInstance* CachedAnimInstance;

	UComboManageComponent* CachedComboManageComponent;

	UWeaponComponent* CachedWeaponComponent;

	UKickComponent* CachedKickComponent;

	UPlayerAttributeComponent* CachedAttributeComponent;

	//only visible in Detail Panel
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE EPlayerCombatState GetPlayerCombatState() const { return PlayerCombatState; }
	FORCEINLINE void SetPlayerCombatState(EPlayerCombatState State) { PlayerCombatState = State; }

	FORCEINLINE EPlayerMoveState GetPlayerMoveState() const { return PlayerMoveState; }
	FORCEINLINE void SetPlayerMoveState(EPlayerMoveState State) { PlayerMoveState = State; }


};
