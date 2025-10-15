// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/Types/PlayerType.h"
#include "PlayerAnimInstance.generated.h"

class UCharacterMovementComponent;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackMontageEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveEnable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionEnable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionDisable);

/**
 * 
 */
UCLASS()
class RPG_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAnimMontage(int32 Num);

	void PlayChainAttackMontage(int32 Num, FName Section);

	UPROPERTY(BlueprintAssignable)
	FOnAttackMontageEnd OnAttackMontageEnd;

	UPROPERTY(BlueprintAssignable)
	FOnMoveEnable OnMoveEnable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionEnable OnWeaponCollisionEnable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionDisable OnWeaponCollisionDisable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionEnable OnKickCollisionEnable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionDisable OnKickCollisionDisable;


protected:

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_MoveEnable();

	UFUNCTION()
	void AnimNotify_WeaponCollisionEnable();

	UFUNCTION()
	void AnimNotify_WeaponCollisionDisable();

	UFUNCTION()
	void AnimNotify_KickCollisionEnable();

	UFUNCTION()
	void AnimNotify_KickCollisionDisable();

	UPROPERTY(BlueprintReadOnly)
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	//UPROPERTY(BlueprintReadOnly, Category="Movement | Player State")
	//EPlayerState PlayerState;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackA;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackB;

	UPROPERTY(EditAnywhere, Category = "Combo")
	UAnimMontage* ComboA;

	UPROPERTY(EditAnywhere, Category = "Combo")
	UAnimMontage* ComboB;

	UPROPERTY(EditAnywhere, Category = "Stun")
	UAnimMontage* StunA;


public:
	FORCEINLINE float GetGroundSpeed() const { return GroundSpeed; }
	
};
