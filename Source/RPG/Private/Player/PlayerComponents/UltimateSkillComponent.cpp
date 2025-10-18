// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerComponents/UltimateSkillComponent.h"

#include "Player/Types/PlayerType.h"

// Sets default values for this component's properties
UUltimateSkillComponent::UUltimateSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUltimateSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUltimateSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUltimateSkillComponent::TimerStart(EUltimateSkill* SelectedSkill)
{
	bIsSkillActivated = true;
	float SkillAActiveTime = 5.f;
	if (*SelectedSkill == EUltimateSkill::SkillA)
	{
		GetWorld()->GetTimerManager().SetTimer(
			SkillActiveTimer, this, &UUltimateSkillComponent::SkillTimeOver, SkillAActiveTime, false
		); //last parameter false-> call Timer just once
		UE_LOG(LogTemp, Warning, TEXT("Skill A Activated"));
	}
}

void UUltimateSkillComponent::SkillTimeOver()
{
	//VFX해제
	bIsSkillActivated = false;
	UE_LOG(LogTemp, Warning, TEXT("Skill Time Over"));
}

void UUltimateSkillComponent::Skill1(float AttackValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Action Pressed"));
	if (AttackValue == 1.f)
	{
		//포인터라서 이렇게 접근해야함
		UE_LOG(LogTemp, Warning, TEXT("Attack Value: %f"), AttackValue);
		UE_LOG(LogTemp, Warning, TEXT("Combo AnimNum: %d"), (*CachedComboPatterns)[0].AnimMontageNum);
		OnSkill1Attack.Broadcast((*CachedComboPatterns)[0].AnimMontageNum, (*CachedComboPatterns)[0].ComboAttackMultiplier);
	}
	else if (AttackValue == -1.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Value: %f"), AttackValue);
		OnSkill1Attack.Broadcast((*CachedComboPatterns)[1].AnimMontageNum, (*CachedComboPatterns)[1].ComboAttackMultiplier);
	}
}

