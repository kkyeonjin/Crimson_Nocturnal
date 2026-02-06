// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Boss/BossSFXComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBossSFXComponent::UBossSFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

