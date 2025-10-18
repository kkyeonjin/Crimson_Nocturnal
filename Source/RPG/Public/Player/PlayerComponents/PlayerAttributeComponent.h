// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/AttributeInterface.h"
#include "Player/Types/PlayerStat.h"
#include "PlayerAttributeComponent.generated.h"

class UEnemyAttributeComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UPlayerAttributeComponent : public UActorComponent, public IAttributeInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAttributeComponent();
	
	virtual int32 CalcDamage() override;
	virtual void ApplyDamage() override;

	void SetEnemyAttributeComponent(AActor* Target);
	void AddEXP(int32 EXPValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	UDataTable* PlayerStatTable;

private:
	//to get enemy's DEF value for Damage Calculation
	UEnemyAttributeComponent* EnemyAttributeComponent;

	int32 ATK;
	int32 DEF;
	int32 CurrentHP;
	int32 CurrentBP;
	int32 MaxBP;
	int32 EXP;
	int32 Level;
	int32 SkillPoint;

	float AttackMultiplier;

	int32 BPtoGet = 0;

	//Leveling Variables
	int32 BaseEXP;
	float GrowthRate;

	void InitializePlayerStat();

public:
	FORCEINLINE virtual int32 GetATK() override { return ATK; }
	FORCEINLINE virtual int32 GetDEF() override { return DEF; }
	FORCEINLINE virtual int32 GetCurrentHP() override { return CurrentHP; }

	FORCEINLINE int32 ADDCurrentHP(int32 HPtoAdd) { return CurrentHP += HPtoAdd; }

	FORCEINLINE void SetAttackMultiplier(float CurrentAttackMultiplier) { AttackMultiplier = CurrentAttackMultiplier; }
	FORCEINLINE void SetBPtoGet(int32 BPValue) { BPtoGet = BPValue; }
	FORCEINLINE int32 GetRequiredEXPForNextLevel() { return FMath::FloorToInt32(BaseEXP * FMath::Pow(GrowthRate, Level - 1)); }
	//FORCEINLINE void AddEXP(int32 EXPValue) { EXP += EXPValue; }
};
