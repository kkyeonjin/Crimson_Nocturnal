// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define HP_THRESHOLD_1 145
#define HP_THRESHOLD_2 65

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttributeInterface.h"
#include "BossAttributeComponent.generated.h"

class ABoss;
class UPlayerAttributeComponent;
class APlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UBossAttributeComponent : public UActorComponent, public IAttributeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBossAttributeComponent();

	virtual int32 CalcDamage();
	virtual void ApplyDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* BossStatTable;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int32 CurrentHP;
	int32 MaxHP;
	int32 DEF;
	TArray<int32> ATK;
	int32 PhaseNum;

	UPlayerAttributeComponent* PlayerAttributeComponent;
	ABoss* Owner;

	bool IsInfected = false;

	void SetPhaseNum();

public:
	FORCEINLINE virtual int32 GetATK() { return ATK[PhaseNum]; }
	FORCEINLINE virtual int32 GetDEF() { return DEF; }
	FORCEINLINE virtual int32 GetCurrentHP() { return CurrentHP; }
	FORCEINLINE virtual int32 GetMaxHP() { return MaxHP; }

	FORCEINLINE bool ReadyToStartUlt() { return CurrentHP <= MaxHP / 10; }

	FORCEINLINE void SetIsInfected(bool bIsInfected) { IsInfected = bIsInfected; }

};
