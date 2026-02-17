// Manage SFX playing of boss

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "BossSFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UBossSFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBossSFXComponent();

	void PlayBattleStartSFX();
	void PlayHitSFX();
	void PlayDeathSFX();
	void PlayUltSFX();
	void PlayPhaseSkillSFX(int32 PhaseNum);


private:
	// battle Start
	UPROPERTY(EditAnywhere, Category = "Audio|BattleStart")
	USoundCue* BattleStartSFX;

	// Phase SFX
	UPROPERTY(EditAnywhere, Category = "Audio|PhaseSkill")
	USoundCue* Phase1SFX;
	UPROPERTY(EditAnywhere, Category = "Audio|PhaseSkill")
	USoundCue* Phase2SFX;
	UPROPERTY(EditAnywhere, Category = "Audio|PhaseSkill")
	USoundCue* Phase3SFX;

	// Ult Skill SFX
	UPROPERTY(EditAnywhere, Category = "Audio|UltSkill")
	USoundCue* UltSkillSFX;
	
	UPROPERTY(EditAnywhere, Category = "Audio|Damage")
	USoundCue* HitSFX;
	UPROPERTY(EditAnywhere, Category = "Audio|Damage")
	USoundCue* DeathSFX;

	void PlaySoundAtPlayer(USoundCue* SoundCue);

};
