// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Player/Types/ComboInput.h"
#include "UltimateSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkill1Attack, int32, AnimNum, float, AttackMultiplier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUltTimeOver);

//struct FComboPattern;
enum class EUltimateSkill : uint8;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UUltimateSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUltimateSkillComponent();

	//Timer Manage Func
	void TimerStart(EUltimateSkill* SelectedSkill);

	void SkillTimeOver();

	//Skill 2/3은 여기에 함수 없어도 ㄱㅊ음 
	void Skill1(float AttackValue);

	UPROPERTY()
	FOnSkill1Attack OnSkill1Attack;

	UPROPERTY()
	FOnUltTimeOver OnUltTimeOver;

private:
	FTimerHandle SkillActiveTimer;
	float SkillAActiveTime = 6.f; //Default
	float SkillBActiveTime = 5.f; //Default
	float SkillCActiveTime = 7.f; //Default

	int32 XSettingForSkillA = 0; //Default
	int32 YSettingForSkillA = 0; //Default

	TArray<FComboPattern>* CachedComboPatterns;

	bool bIsSkillActivated = false;

public:
	FORCEINLINE void SetCachedComboPatters(TArray<FComboPattern>* ComboPatterns) { CachedComboPatterns = ComboPatterns; }
	FORCEINLINE bool IsSkillActivated() { return bIsSkillActivated; }
	FORCEINLINE void IncreaseSkillTime() { SkillAActiveTime *= 1.5f; SkillBActiveTime *= 1.5f; SkillCActiveTime *= 1.5f; }

	//For SkillA
	FORCEINLINE void SetKeyBindingXInSkillA(int32 ComboNum) { XSettingForSkillA = ComboNum; }
	FORCEINLINE void SetKeyBindingYInSkillA(int32 ComboNum) { YSettingForSkillA = ComboNum; }

};
