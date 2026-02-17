#include "Character/Enemy/EnemyAnimInstance.h"
#include "Character/Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyAnimInstance::AnimNotify_AttackEnd()
{
	OnEnemyAttackMontageEnd.Broadcast();
	IsAttacking = false;
}

void UEnemyAnimInstance::AnimNotify_CollisionEnable()
{
	OnEnemyCollisionEnable.Broadcast();
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
	}
}

void UEnemyAnimInstance::PlayAnimMontage(int32 Num)
{
	switch (Num)
	{
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

