// Boss stats with phase transitions (70%/20% HP thresholds)
// Broadcasts OnHpThreshold for phase changes/QTE/crystal events

#pragma once
#define HP_THRESHOLD_1 700
#define HP_THRESHOLD_2 200

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Interface/AttributeInterface.h"
#include "BossAttributeComponent.generated.h"

class ABoss;
class UPlayerAttributeComponent;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHpThreshold, int32, ThresholdStep);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UBossAttributeComponent : public UActorComponent, public IAttributeInterface
{
	GENERATED_BODY()

public:
	UBossAttributeComponent();

	// IAttributeInterface
	virtual int32 CalcDamage();
	virtual void ApplyDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* BossStatTable;


	// Delegate (send to Boss)
	FOnHpThreshold OnHpThreshold;

protected:
	virtual void BeginPlay() override;

private:
	int32 CurrentHP;
	int32 MaxHP;
	int32 DEF;

	UPROPERTY(EditAnywhere)
	int32 ATK;

	int32 PhaseNum;

	UPlayerAttributeComponent* CachedPlayerAttributeComponent;
	ABoss* Owner;

	bool IsInfected = false;

	void CachePlayerAttribute();

public:
	FORCEINLINE virtual int32 GetATK() { return ATK; }
	FORCEINLINE virtual int32 GetDEF() { return DEF; }
	FORCEINLINE virtual int32 GetCurrentHP() { return CurrentHP; }
	FORCEINLINE virtual int32 GetMaxHP() { return MaxHP; }
	FORCEINLINE bool ReadyToStartUlt() { return CurrentHP <= MaxHP / 10; }
	FORCEINLINE void SetIsInfected(bool bIsInfected) { IsInfected = bIsInfected; }

};
