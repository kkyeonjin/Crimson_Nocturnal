// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Animation/EnemyAnimInstance.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyAnimInstance::AnimNotify_AttackEnd()
{
	OnEnemyAttackMontageEnd.Broadcast();
	//UE_LOG(LogTemp, Warning, TEXT("EnemyAttackEndNotifyRings"));
	IsAttacking = false;
}

void UEnemyAnimInstance::AnimNotify_CollisionEnable()
{
	OnEnemyCollisionEnable.Broadcast();
	//UE_LOG(LogTemp, Warning, TEXT("CollisionEnableNotifyRings"));
}

void UEnemyAnimInstance::AnimNotify_CollisionDisable()
{
	OnEnemyCollisionDisable.Broadcast();
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//find pawn Instance that currently applied on animation blueprint
	Enemy = Cast<AEnemy>(TryGetPawnOwner());
	if (Enemy) //casting can be failed
	{
		EnemyMovement = Enemy->GetCharacterMovement();
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (EnemyMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(EnemyMovement->Velocity);
		//UE_LOG(LogTemp, Warning, TEXT("Ground Speed: %f"), GroundSpeed);
	}
}

void UEnemyAnimInstance::PlayAnimMontage(int32 Num)
{
	switch (Num)
	{
		//0-9: primary animations(attack, hit reaction,,,etc)
		case 0:
			IsAttacking = true;
			Montage_Play(AttackA);
			break;
		case 1:
			Montage_Play(HitReactA);
			break;
		case 2:
			Montage_Play(Death);
			break;
		default:
			break;

	}
}
