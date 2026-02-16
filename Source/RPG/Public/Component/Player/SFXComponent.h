// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "SFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API USFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USFXComponent();

    // ===== Attack SFX =====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Attack")
    USoundCue* Chain1AttackXSFX;  // 첫 번째 체인

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Attack")
    USoundCue* Chain2AttackXSFX;  // 두 번째 체인

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Attack")
    USoundCue* Chain3AttackXSFX;  // 세 번째 체인c

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Attack")
    USoundCue* Chain1AttackYSFX;  // 첫 번째 체인

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Attack")
    USoundCue* Chain2AttackYSFX;  // 두 번째 체인

    // Combo SFX
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Attack")
    USoundCue* ComboAttackASFX;   // 콤보 마무리

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Attack")
    USoundCue* ComboAttackBSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Attack")
    USoundCue* ComboAttackCSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Attack")
    USoundCue* ComboAttackDSFX;

    // ===== Damage SFX =====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Damage")
    USoundCue* HitSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Damage")
    USoundCue* DeathSFX;

    // ===== Ultimate Skill SFX =====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|UltSkill")
    USoundCue* UltASFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|UltSkill")
    USoundCue* UltBSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|UltSkill")
    USoundCue* UltCSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|LevelUp")
    USoundCue* LevelUpSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|BPMax")
    USoundCue* BPMaxSFX;

    // ===== 재생 함수들 =====
    void PlayChainAttackSFX(int32 AnimNum, FName SectionName);
    void PlayComboAttackSFX(int32 ComboNum);
    void PlayHitSFX();
    void PlayDeathSFX();
    void PlayUltSkillSFX(int32 UltSkillIndex);
    void PlayLevelUpSFX();
    void PlayBPMaxSPX();

	
private:
    void PlaySoundAtPlayer(USoundCue* SoundCue);
		
};
