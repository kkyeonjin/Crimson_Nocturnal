/**
 * Enemy stats: ATK/DEF/HP/EXP. Applies SkillC infection debuff (2x damage).
 * Implements IAttributeInterface for Player<->Enemy damage exchange.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttributeInterface.h"
#include "EnemyAttributeComponent.generated.h"

class UPlayerAttributeComponent;
class APlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UEnemyAttributeComponent : public UActorComponent, public IAttributeInterface
{
	GENERATED_BODY()

public:
	UEnemyAttributeComponent();

	// IAttributeInterface
	virtual int32 CalcDamage() override;
	virtual void ApplyDamage() override;

	//send EXPValue to Player when Enemy is dead
	void SendEXPValue();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* EnemyStatTable;

private:
	int32 ATK;
	int32 DEF;
	int32 CurrentHP;
	int32 MaxHP;
	int32 EXPValue;

	FName EnemyTypeName;
	UPlayerAttributeComponent* CachedPlayerAttributeComponent;
	bool IsInfected;

	void CachePlayerAttribute();

public:
	FORCEINLINE virtual int32 GetATK() override { return ATK; }
	FORCEINLINE virtual int32 GetDEF() override { return DEF; }
	FORCEINLINE virtual int32 GetCurrentHP() override { return CurrentHP; }
	FORCEINLINE virtual int32 GetMaxHP() override { return MaxHP; }
	FORCEINLINE void SetIsInfected(bool bIsInfected) { IsInfected = bIsInfected; }


};
