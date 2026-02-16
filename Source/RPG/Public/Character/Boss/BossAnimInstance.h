// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlamActorSpawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossCollisionEnable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossCollisionDisable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDead);

/**
 *
 */
UCLASS()
class RPG_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void PlayBossAnimMontage(int32 AnimNum);

	UPROPERTY()
	FOnBattleStart OnBattleStart;

	UPROPERTY()
	FOnSlamActorSpawn OnSlamActorSpawn;

	UPROPERTY()
	FOnBossCollisionEnable OnBossCollisionEnable;

	UPROPERTY()
	FOnBossCollisionDisable OnBossCollisionDisable;

	UPROPERTY()
	FOnBossDead OnBossDead;

protected:
	UFUNCTION()
	void AnimNotify_BattleStart();

	UFUNCTION()
	void AnimNotify_SlamActorSpawn();

	UFUNCTION()
	void AnimNotify_CollisionEnable();

	UFUNCTION()
	void AnimNotify_CollisionDisable();

	UFUNCTION()
	void AnimNotify_BossDead();


private:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Phase1;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Phase2;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Phase3;

	UPROPERTY(EditAnywhere)
	UAnimMontage* UltMotion;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitReact;

	UPROPERTY(EditAnywhere)
	UAnimMontage* BattleStartMotion;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Death;
};
