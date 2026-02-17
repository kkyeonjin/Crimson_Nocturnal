// Manage SFX playing of player

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
	USFXComponent();

    void PlayChainAttackSFX(int32 AnimNum, FName SectionName);
    void PlayComboAttackSFX(int32 ComboNum);
    void PlayHitSFX();
    void PlayDeathSFX();
    void PlayUltSkillSFX(int32 UltSkillIndex);
    void PlayLevelUpSFX();
    void PlayBPMaxSPX();

	
private:
    // Sword Attack
    UPROPERTY(EditAnywhere, Category = "Audio|Attack")
    USoundCue* Chain1AttackXSFX;  
    UPROPERTY(EditAnywhere, Category = "Audio|Attack")
    USoundCue* Chain2AttackXSFX;  
    UPROPERTY(EditAnywhere, Category = "Audio|Attack")
    USoundCue* Chain3AttackXSFX;  

    // Kick Attack
    UPROPERTY(EditAnywhere, Category = "Audio|Attack")
    USoundCue* Chain1AttackYSFX;  
    UPROPERTY(EditAnywhere, Category = "Audio|Attack")
    USoundCue* Chain2AttackYSFX;  

    // Combo SFX
    UPROPERTY(EditAnywhere, Category = "Audio|Attack")
    USoundCue* ComboAttackASFX;   
    UPROPERTY(EditAnywhere, Category = "Audio|Attack")
    USoundCue* ComboAttackBSFX;
    UPROPERTY(EditAnywhere, Category = "Audio|Attack")
    USoundCue* ComboAttackCSFX;
    UPROPERTY(EditAnywhere, Category = "Audio|Attack")
    USoundCue* ComboAttackDSFX;

    
    UPROPERTY(EditAnywhere, Category = "Audio|Damage")
    USoundCue* HitSFX;
    UPROPERTY(EditAnywhere, Category = "Audio|Damage")
    USoundCue* DeathSFX;

    // Ult Skill
    UPROPERTY(EditAnywhere, Category = "Audio|UltSkill")
    USoundCue* UltASFX;
    UPROPERTY(EditAnywhere, Category = "Audio|UltSkill")
    USoundCue* UltBSFX;
    UPROPERTY(EditAnywhere, Category = "Audio|UltSkill")
    USoundCue* UltCSFX;


    UPROPERTY(EditAnywhere, Category = "Audio|LevelUp")
    USoundCue* LevelUpSFX;
    UPROPERTY(EditAnywhere, Category = "Audio|BPMax")
    USoundCue* BPMaxSFX;

    void PlaySoundAtPlayer(USoundCue* SoundCue);
		
};
