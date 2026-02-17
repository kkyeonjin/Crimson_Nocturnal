#include "Component/Player/SFXComponent.h"
#include "Kismet/GameplayStatics.h"

USFXComponent::USFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USFXComponent::PlayChainAttackSFX(int32 AnimNum, FName SectionName)
{
	if (SectionName == TEXT("Chain1"))
	{
		if (AnimNum == 0)
		{
			PlaySoundAtPlayer(Chain1AttackXSFX);
		}
		else if (AnimNum == 1)
		{
			PlaySoundAtPlayer(Chain1AttackYSFX);
		}
	}
	else if (SectionName == TEXT("Chain2"))
	{
		if (AnimNum == 0)
		{
			PlaySoundAtPlayer(Chain2AttackXSFX);
		}
		else if (AnimNum == 1)
		{
			PlaySoundAtPlayer(Chain2AttackYSFX);
		}
	}
	else if (SectionName == TEXT("Chain3"))
	{
		PlaySoundAtPlayer(Chain3AttackXSFX);
	}

}

void USFXComponent::PlayComboAttackSFX(int32 ComboNum)
{
	switch (ComboNum)
	{
		case 0:
			PlaySoundAtPlayer(ComboAttackASFX);
			break;
		case 1:
			PlaySoundAtPlayer(ComboAttackBSFX);
			break;
		case 2:
			PlaySoundAtPlayer(ComboAttackCSFX);
			break;
		case 3:
			PlaySoundAtPlayer(ComboAttackDSFX);
			break;
		default:
			break;
	}
}

void USFXComponent::PlayHitSFX()
{
	PlaySoundAtPlayer(HitSFX);
}

void USFXComponent::PlayDeathSFX()
{
	PlaySoundAtPlayer(DeathSFX);
}

void USFXComponent::PlayUltSkillSFX(int32 UltSkillIndex)
{
	switch (UltSkillIndex)
	{
		case 0:
			PlaySoundAtPlayer(UltASFX);
			break;
		case 1:
			PlaySoundAtPlayer(UltBSFX);
			break;
		case 2:
			PlaySoundAtPlayer(UltCSFX);
			break;
		default:
			break;
	}
}

void USFXComponent::PlayLevelUpSFX()
{
	PlaySoundAtPlayer(LevelUpSFX);
}

void USFXComponent::PlayBPMaxSPX()
{
	PlaySoundAtPlayer(BPMaxSFX);
}

void USFXComponent::PlaySoundAtPlayer(USoundCue* SoundCue)
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundCue, GetOwner()->GetActorLocation());
}

