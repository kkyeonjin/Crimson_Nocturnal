// Spawning particle & decals for Boss Phase1 Attack (AoE)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"
#include "MagicCircleAoE.generated.h"

class USphereComponent;

UCLASS()
class RPG_API AMagicCircleAoE : public AActor
{
    GENERATED_BODY()

public:
    AMagicCircleAoE();

    UFUNCTION(BlueprintCallable)
    void InitializeCircle();

protected:
    virtual void BeginPlay() override;

    // MagicCircle
    UPROPERTY(EditAnywhere, Category = "VFX")
    TSubclassOf<AActor> DecalClass;

    // FireStorm
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<AActor> ParticleEffectClass;

    // 경고 시간
    UPROPERTY(EditAnywhere, Category = "Settings")
    float WarningDuration = 2.0f;

    // 경고 시간
    UPROPERTY(EditAnywhere, Category = "Settings")
    float ExplosionDuration = 2.0f;

    // Center of the fight zone (for setting spawn points)
    UPROPERTY(EditAnywhere, Category = "Settings")
    FVector StageCenter;

private:
    FTimerHandle ExplodeTimer;

    FTimerHandle WarningTimerHandle;

    void OnWarningEnd();

    void OnExplosionEnd();

    void SetRandomPoints();

    TArray<FVector> RandomPoints;

    FTimerHandle PulseTimer;
    float PulseAlpha = 0.0f;
};
