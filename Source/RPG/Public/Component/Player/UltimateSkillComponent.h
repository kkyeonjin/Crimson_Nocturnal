//Ultimate skill system: 3 skills with timer-based activation.
// UltSkillA: Custom combo keybinding / UltSkillB: Lifesteal (passive) / UltSkillC: Infection (passive broadcast)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Player/Types/ComboInput.h"
#include "UltimateSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkill1Attack, int32, AnimNum, float, AttackMultiplier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUltTimeOver);

enum class EUltimateSkill : uint8;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UUltimateSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUltimateSkillComponent();

	//Timer Manage Function
	void TimerStart(EUltimateSkill* SelectedSkill);

	void SkillTimeOver();

	//Handle UltSkillA input (custom combo playback)
	void Skill1(float AttackValue);

	//Delegates
	UPROPERTY()
	FOnSkill1Attack OnSkill1Attack;

	UPROPERTY()
	FOnUltTimeOver OnUltTimeOver;

private:
	FTimerHandle SkillActiveTimer;

	// UltSkill durations(upgradable)
	float SkillAActiveTime = 6.f; 
	float SkillBActiveTime = 5.f; 
	float SkillCActiveTime = 7.f; 


	// UltSkillA combo bindings (0-3 = ComboA-D)
	int32 XSettingForSkillA = 0; 
	int32 YSettingForSkillA = 0; 

	TArray<FComboPattern>* CachedComboPatterns;

	bool bIsSkillActivated = false;

public:
	FORCEINLINE void SetCachedComboPatters(TArray<FComboPattern>* ComboPatterns) { CachedComboPatterns = ComboPatterns; }
	FORCEINLINE bool IsSkillActivated() { return bIsSkillActivated; }
	FORCEINLINE void IncreaseSkillTime() { SkillAActiveTime *= 1.5f; SkillBActiveTime *= 1.5f; SkillCActiveTime *= 1.5f; }

	//For UltSkillA
	FORCEINLINE void SetKeyBindingXInSkillA(int32 ComboNum) { XSettingForSkillA = ComboNum; }
	FORCEINLINE void SetKeyBindingYInSkillA(int32 ComboNum) { YSettingForSkillA = ComboNum; }

};
