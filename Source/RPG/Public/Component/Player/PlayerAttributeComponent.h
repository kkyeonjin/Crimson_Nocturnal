// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttributeInterface.h"
//#include "Player/Types/PlayerStat.h"
#include "Game/SaveGameData.h"
#include "Character/Player/Types/LevelData.h"
#include "PlayerAttributeComponent.generated.h"

class UEnemyAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBPUpdate, int32, CurrentBP, int32, MaxBP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBPMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPUpdate, int32, CurrentHP, int32, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillTreeUISet, int32, SkillPoint, int32, UltNum, TArray<int32>, ActivatedSkills);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUp, int32, CurrentLevel, int32, SkillPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillPointUpdate, int32, SkillPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageCalculated, int32, Damage);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	//For SkillTree
	void UpdateStatsBySkill(int SkillNum);

	UPROPERTY()
	FOnBPUpdate OnBPUpdate;

	UPROPERTY()
	FOnHPUpdate OnHPUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnSkillTreeUISet OnSkillTreeUISet;

	UPROPERTY()
	FOnLevelUp OnLevelUp;

	UPROPERTY()
	FOnDamageCalculated OnDamageCalculated;

	UPROPERTY()
	FOnSkillPointUpdate OnSkillPointUpdate;

	UPROPERTY()
	FOnBPMax OnBPMax;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	UDataTable* PlayerStatTable;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	UDataTable* PlayerLevelTable;

private:
	//to get enemy's DEF value for Damage Calculation
	UEnemyAttributeComponent* EnemyAttributeComponent;

	int32 ATK;
	int32 DEF;
	int32 CurrentHP;
	int32 CurrentBP;
	int32 MaxHP;
	int32 MaxBP = 100;
	int32 CurrentEXP;
	int32 CurrentLevel;
	int32 MaxLevel = 5;
	int32 SkillPoint;
	int32 EquippedUlt;
	TArray<int32> ActivatedSkills;

	float AttackMultiplier;

	int32 BPtoGet = 0;

	float ATKMutiplier; //for Skill1
	float DEFMultiplier; //for Skill2
	float MaxHPMultiplier; //for SKill3
	float BPMultiplier; //for Skill4

	//Leveling Variables
	int32 BaseEXP;
	float GrowthRate;

	void InitializePlayerStat();

	FLevelData* GetLevelData(int32 Level);

	void CheckLevelUp();
	void LevelUp();

	//Load & Save
	void LoadPlayerData();
	void SavePlayerData();

	UFUNCTION(BlueprintPure, Category = "Save System")
	bool DoesSaveGameExist() const;

public:
	FORCEINLINE virtual int32 GetATK() override { return ATK; }
	FORCEINLINE virtual int32 GetDEF() override { return DEF; }
	FORCEINLINE virtual int32 GetCurrentHP() override { return CurrentHP; }
	FORCEINLINE virtual int32 GetMaxHP() override { return MaxHP; }
	FORCEINLINE virtual int32 GetCurrentBP() { return CurrentBP; }
	FORCEINLINE virtual int32 GetMaxBP() { return MaxBP; }

	FORCEINLINE int32 GetCurrentLevel() { return CurrentLevel; }

	FORCEINLINE int32 ADDCurrentHP(int32 HPtoAdd) { return CurrentHP += HPtoAdd; }

	FORCEINLINE void SetAttackMultiplier(float CurrentAttackMultiplier) { AttackMultiplier = CurrentAttackMultiplier; }
	FORCEINLINE void SetBPtoGet(int32 BPValue) { BPtoGet = BPValue; }
	FORCEINLINE int32 GetSkillPoint() { return SkillPoint; }
	//FORCEINLINE void AddEXP(int32 EXPValue) { EXP += EXPValue; }
};
