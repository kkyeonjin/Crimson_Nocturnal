// DecalMaterial that is spawned when Boss Phase1 Attack(AoE) starts
// must be spawned before the firestorm actor's spawning

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class RPG_API AMagicCircle : public AActor
{
    GENERATED_BODY()

public:
    AMagicCircle();

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UDecalComponent* Decal;

    UFUNCTION(BlueprintCallable, Category = "Magic Circle")
    void InitializeDecal(UDecalComponent* MagicCircle);


private:
    void DestroyDecal();
    FTimerHandle DestroyTimer;

};

