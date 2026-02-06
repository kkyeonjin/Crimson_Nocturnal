// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this component's properties
	UBossSFXComponent();

	//battle Start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|BattleStart")
	USoundCue* BattleStartSFX;

	//Phase SFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|PhaseSkill")
	USoundCue* Phase1SFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|PhaseSkill")
	USoundCue* Phase2SFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|PhaseSkill")
	USoundCue* Phase3SFX;

	//Ult Skill SFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|UltSkill")
	USoundCue* UltSkillSFX;

	// ===== Damage SFX =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Damage")
	USoundCue* HitSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Damage")
	USoundCue* DeathSFX;

	// ===== 재생 함수들 ====
	void PlayBattleStartSFX();
	void PlayHitSFX();
	void PlayDeathSFX();
	void PlayUltSFX();
	void PlayPhaseSkillSFX(int32 PhaseNum);

private:
	void PlaySoundAtPlayer(USoundCue* SoundCue);

};
