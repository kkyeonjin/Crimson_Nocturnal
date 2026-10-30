// Player stats management: HP/BP/EXP/Level/Skills/Save
// Handles damage calc, level-up, persistent data via GameInstance

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttributeInterface.h"
#include "Game/SaveGameData.h"
#include "Character/Player/Types/LevelData.h"
#include "PlayerAttributeComponent.generated.h"

class UEnemyAttributeComponent;
class UBossAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBPUpdate, int32, CurrentBP, int32, MaxBP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBPMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPUpdate, int32, CurrentHP, int32, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnSkillTreeUISet, int32, SkillPoint, int32, UltNum, TArray<int32>, ActivatedSkills, int32, Ult1XSetting, int32, Ult1YSetting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainHUDSet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUp, int32, CurrentLevel, int32, SkillPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpdate, int32, CurrentLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillPointUpdate, int32, SkillPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageCalculated, int32, Damage);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UPlayerAttributeComponent : public UActorComponent, public IAttributeInterface
{
	GENERATED_BODY()

public:
	UPlayerAttributeComponent();

	// IAttributeInterface
	virtual int32 CalcDamage() override;
	virtual void ApplyDamage() override;

	// Set target enemy/boss for damage calculation
	void SetEnemyAttributeComponent(AActor* Target);

	void AddEXP(int32 EXPValue);

	// Apply skill upgrade effects (ATK/DEF/HP/BP multipliers)
	void UpdateStatsBySkill(int SkillNum);


	// Save/Load (called by GameInstance)
	UFUNCTION()
	void LoadPlayerData();

	UFUNCTION()
	void SavePlayerData();

	UFUNCTION()
	void RequestSave();

	UFUNCTION()
	void RestoreRunTimeData(USaveGameData* RuntimeSaveGameData);

	// UI Initialize
	void BroadcastInitialUIState();

	//callbackFUNC
	UFUNCTION()
	void InvincibleStart();

	UFUNCTION()
	void InvincibleEnd();


	// Delegates (PlayerCharacter/HUD binding)
	UPROPERTY()
	FOnBPUpdate OnBPUpdate;

	UPROPERTY()
	FOnHPUpdate OnHPUpdate;

	UPROPERTY()
	FOnSkillTreeUISet OnSkillTreeUISet;

	UPROPERTY()
	FOnMainHUDSet OnMainHUDSet;

	UPROPERTY()
	FOnLevelUp OnLevelUp;

	UPROPERTY()
	FOnLevelUpdate OnLevelUpdate;

	UPROPERTY()
	FOnDamageCalculated OnDamageCalculated;

	UPROPERTY()
	FOnSkillPointUpdate OnSkillPointUpdate;

	UPROPERTY()
	FOnBPMax OnBPMax;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	UDataTable* PlayerLevelTable;

private:
	//to get enemy's DEF value for Damage Calculation
	UEnemyAttributeComponent* EnemyAttributeComponent;
	UBossAttributeComponent* BossAttributeComponent;
	//store Enemy or Boss's DEF value for CalcDamage
	int32 TargetDEF; 


	// Core stats
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
	TArray<int32> ActivatedSkills = { 0, 0, 0, 0, 0 };
	int32 Ult1XSetting;
	int32 Ult1YSetting;


	// Combat modifiers
	float AttackMultiplier = 1.f;
	int32 BPtoGet = 0;
	bool IsBPMax = false;

	float ATKMutiplier; //for Skill1
	float DEFMultiplier; //for Skill2
	float MaxHPMultiplier; //for SKill3
	float BPMultiplier; //for Skill4


	//Leveling Variables
	int32 BaseEXP;
	float GrowthRate;


	//DodgeFlag
	bool IsInvincible = false;


	// Helpers
	void InitializePlayerStat();
	FLevelData* GetLevelData(int32 Level);
	void CheckLevelUp();
	void LevelUp();

	void SubscribeToGameInstance();

	bool TryRestoreRuntimeData();
	bool TryLoadSavedData();

	void BindGameModeEvents();


public:
	FORCEINLINE virtual int32 GetATK() override { return ATK; }
	FORCEINLINE virtual int32 GetDEF() override { return DEF; }
	FORCEINLINE virtual int32 GetCurrentHP() override { return CurrentHP; }
	FORCEINLINE virtual int32 GetMaxHP() override { return MaxHP; }
	FORCEINLINE virtual int32 GetCurrentBP() { return CurrentBP; }
	FORCEINLINE virtual int32 GetMaxBP() { return MaxBP; }
	FORCEINLINE void UseBPforUlt() { CurrentBP = 0; IsBPMax = false; }
	FORCEINLINE int32 GetCurrentLevel() { return CurrentLevel; }
	FORCEINLINE void ADDCurrentHP(int32 HPtoAdd) { CurrentHP = FMath::Clamp(CurrentHP + HPtoAdd, 0, MaxHP); }
	FORCEINLINE void SetAttackMultiplier(float CurrentAttackMultiplier) { AttackMultiplier = CurrentAttackMultiplier; }
	FORCEINLINE void SetBPtoGet(int32 BPValue) { BPtoGet = BPValue; }
	FORCEINLINE int32 GetSkillPoint() { return SkillPoint; }
	FORCEINLINE int32 GetUltSkill() { return EquippedUlt; }
	FORCEINLINE void SetUltSkill(int32 UltNum) { EquippedUlt = UltNum; }
	FORCEINLINE void SetUlt1XSetting(int32 Num) { Ult1XSetting = Num; }
	FORCEINLINE void SetUlt1YSetting(int32 Num) { Ult1YSetting = Num; }
};
