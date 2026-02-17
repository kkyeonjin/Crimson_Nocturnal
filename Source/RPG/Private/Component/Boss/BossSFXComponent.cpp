#include "Component/Boss/BossSFXComponent.h"
#include "Kismet/GameplayStatics.h"

UBossSFXComponent::UBossSFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBossSFXComponent::PlayBattleStartSFX()
{
	PlaySoundAtPlayer(BattleStartSFX);
}

void UBossSFXComponent::PlayHitSFX()
{
	PlaySoundAtPlayer(HitSFX);
}

void UBossSFXComponent::PlayDeathSFX()
{
	PlaySoundAtPlayer(DeathSFX);
}

void UBossSFXComponent::PlayUltSFX()
{
	PlaySoundAtPlayer(UltSkillSFX);
}

void UBossSFXComponent::PlayPhaseSkillSFX(int32 PhaseNum)
{
	switch (PhaseNum)
	{
		case 1:
			PlaySoundAtPlayer(Phase1SFX);
			break;
		case 2:
			PlaySoundAtPlayer(Phase2SFX);
			break;
		case 3:
			PlaySoundAtPlayer(Phase3SFX);
			break;
		default:
			break;
	}
}

void UBossSFXComponent::PlaySoundAtPlayer(USoundCue* SoundCue)
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundCue, GetOwner()->GetActorLocation());
}

