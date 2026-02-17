#include "Component/Player/UltimateSkillComponent.h"
#include "Character/Player/Types/PlayerTypes.h"

UUltimateSkillComponent::UUltimateSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUltimateSkillComponent::TimerStart(EUltimateSkill* SelectedSkill)
{
	bIsSkillActivated = true;

	if (*SelectedSkill == EUltimateSkill::SkillA)
	{
		GetWorld()->GetTimerManager().SetTimer(
			SkillActiveTimer, this, &UUltimateSkillComponent::SkillTimeOver, SkillAActiveTime, false
		); 
	}
	else if (*SelectedSkill == EUltimateSkill::SkillB) 
	{
		GetWorld()->GetTimerManager().SetTimer(
			SkillActiveTimer, this, &UUltimateSkillComponent::SkillTimeOver, SkillBActiveTime, false
		);

	}
	else if (*SelectedSkill == EUltimateSkill::SkillC) 
	{
		GetWorld()->GetTimerManager().SetTimer(
			SkillActiveTimer, this, &UUltimateSkillComponent::SkillTimeOver, SkillCActiveTime, false
		);
	}
}

void UUltimateSkillComponent::SkillTimeOver()
{
	OnUltTimeOver.Broadcast();
	bIsSkillActivated = false;
}

//take AttackValue(X/Y)
void UUltimateSkillComponent::Skill1(float AttackValue)
{
	if (!CachedComboPatterns || CachedComboPatterns->IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ComboPatterns not set for SkillA"));
		return;
	}

	//X Button
	if (AttackValue == 1.f)
	{
		// CachedComboPatterns : pointer
		UE_LOG(LogTemp, Warning, TEXT("Attack Value: %f"), AttackValue);
		UE_LOG(LogTemp, Warning, TEXT("Combo AnimNum: %d"), (*CachedComboPatterns)[XSettingForSkillA].AnimMontageNum);
		OnSkill1Attack.Broadcast((*CachedComboPatterns)[XSettingForSkillA].AnimMontageNum, (*CachedComboPatterns)[XSettingForSkillA].ComboAttackMultiplier);
	}
	//Y Button
	else if (AttackValue == -1.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Value: %f"), AttackValue);
		OnSkill1Attack.Broadcast((*CachedComboPatterns)[YSettingForSkillA].AnimMontageNum, (*CachedComboPatterns)[YSettingForSkillA].ComboAttackMultiplier);
	}
}

