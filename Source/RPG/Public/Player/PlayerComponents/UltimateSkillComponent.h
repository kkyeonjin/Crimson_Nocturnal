// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Types/ComboInput.h"
#include "UltimateSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkill1Attack, int32, AnimNum, float, AttackMultiplier);

//struct FComboPattern;
enum class EUltimateSkill : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UUltimateSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUltimateSkillComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Timer Manage Func
	void TimerStart(EUltimateSkill* SelectedSkill);

	void SkillTimeOver();

	void Skill1(float AttackValue);

	UPROPERTY()
	FOnSkill1Attack OnSkill1Attack;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;




private:
	//Rain Particle (Niagara)

	FTimerHandle SkillActiveTimer;

	TArray<FComboPattern>* CachedComboPatterns;

	bool bIsSkillActivated = false;

public:
	FORCEINLINE void SetCachedComboPatters(TArray<FComboPattern>* ComboPatterns) { CachedComboPatterns = ComboPatterns; }
	FORCEINLINE bool IsSkillActivated() { return bIsSkillActivated; }
		
};
