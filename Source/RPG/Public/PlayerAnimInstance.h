// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerType.h"
#include "PlayerAnimInstance.generated.h"

class UCharacterMovementComponent;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackMontageEnd);

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

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UPROPERTY(BlueprintAssignable)
	FOnAttackMontageEnd OnAttackMontageEnd;

	UPROPERTY(BlueprintReadOnly)
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category="Movement | Player State")
	EPlayerState PlayerState;

public:
	FORCEINLINE float GetGroundSpeed() const { return GroundSpeed; }
	
};
