// Manage animations montage playing of enemy

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class UCharacterMovementComponent;
class AEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyAttackMontageEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyCollisionEnable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyCollisionDisable);

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
	FOnEnemyCollisionEnable OnEnemyCollisionEnable;
	FOnEnemyCollisionDisable OnEnemyCollisionDisable;


protected:
	// Callback funcs by animnotify
	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_CollisionEnable();

	UFUNCTION()
	void AnimNotify_CollisionDisable();

	UPROPERTY(BlueprintReadWrite)
	float GroundSpeed;
	UPROPERTY(BlueprintReadWrite)
	bool IsAttacking;


private:
	UPROPERTY()
	AEnemy* Enemy;

	UPROPERTY()
	UCharacterMovementComponent* EnemyMovement;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackA;

	UPROPERTY(EditAnywhere, Category = "Hit Reaction")
	UAnimMontage* HitReactA;

	UPROPERTY(EditAnywhere, Category = "Death")
	UAnimMontage* Death;

};
