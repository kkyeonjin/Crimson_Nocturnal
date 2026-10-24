// Service node for enemy AI that calculating dist between player & enemy (Tick)
// Perception is not enough for appropriate movement of AI

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateDistance.generated.h"


UCLASS()
class RPG_API UBTService_UpdateDistance : public UBTService
{
    GENERATED_BODY()

public:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float TickInterval = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AttackRadius = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName TargetActorKey = FName(TEXT("TargetActor"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName IsInAttackRangeKey = FName(TEXT("IsInAttackRange"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName IsPlayerDetectedKey = FName(TEXT("IsPlayerDetected"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName TargetDistanceKey = FName(TEXT("TargetDistance"));

};
