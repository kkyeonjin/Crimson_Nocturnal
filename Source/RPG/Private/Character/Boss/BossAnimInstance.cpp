#include "Character/Boss/BossAnimInstance.h"

void UBossAnimInstance::AnimNotify_BattleStart()
{
	OnBattleStart.Broadcast();
}

void UBossAnimInstance::AnimNotify_SlamActorSpawn()
{
	OnSlamActorSpawn.Broadcast();
}

void UBossAnimInstance::AnimNotify_CollisionEnable()
{
	OnBossCollisionEnable.Broadcast();
}

void UBossAnimInstance::AnimNotify_CollisionDisable()
{
	OnBossCollisionDisable.Broadcast();
}

void UBossAnimInstance::AnimNotify_BossDead()
{
	OnBossDead.Broadcast();
}


void UBossAnimInstance::PlayBossAnimMontage(int32 AnimNum)
{
	switch (AnimNum)
	{
	case 0:
		Montage_Play(BattleStartMotion);
		break;
	case 1:
		Montage_Play(Phase1);
		break;
	case 2:
		Montage_Play(Phase2);
		break;
	case 3:
		Montage_Play(Phase3);
		break;
	case 4:
		Montage_Play(HitReact);
		break;
	case 5:
		Montage_Play(UltMotion);
		break;
	case 6:
		Montage_Play(Death);
		break;
	default:
		break;
	}
}
