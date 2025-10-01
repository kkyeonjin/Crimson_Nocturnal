// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class UCharacterMovementComponent;
class AEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyAttackMontageEnd);

/**
 * 
 */
UCLASS()
class RPG_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void PlayAnimMontage(int32 Num);

	FOnEnemyAttackMontageEnd OnEnemyAttackMontageEnd;


protected:

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UPROPERTY(BlueprintReadOnly)
	AEnemy* Enemy;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* EnemyMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackA;

	UPROPERTY(EditAnywhere, Category = "Hit Reaction")
	UAnimMontage* HitReactA;

private:
	//void AnimNotify_AttackEnd();


public:
	//FORCEINLINE float GetGroundSpeed() const { return GroundSpeed; }
};
