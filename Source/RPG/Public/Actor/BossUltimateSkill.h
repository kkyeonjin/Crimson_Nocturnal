// UltimateSkillActor used in boss fight

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossUltimateSkill.generated.h"

class APlayerCharacter;

UCLASS()
class RPG_API ABossUltimateSkill : public AActor
{
	GENERATED_BODY()
	
public:
	ABossUltimateSkill();

	void StartUltSkill();

	UFUNCTION(BlueprintCallable, Category = "Ultimate|Mechanic")
	void TeleportPlayerToCenter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate|References")
	APlayerCharacter* TargetPlayer = nullptr;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate|Settings", meta = (AllowPrivateAccess))
	float TeleportHeight = 100.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DarkStormActorClass;

	FTimerHandle StormTimer;

public:
	FORCEINLINE void SetTargetPlayer(APlayerCharacter* NewPlayer) { TargetPlayer = NewPlayer; }

};
