// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateDistance.generated.h"

/**
 *
 */
UCLASS()
class RPG_API UBTService_UpdateDistance : public UBTService
{
    GENERATED_BODY()

public:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    // Service가 활성화될 때 실행 간격 (초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float TickInterval = 0.1f;

    // 공격 사정거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AttackRadius = 300.0f;

    // Blackboard 키
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName TargetActorKey = FName(TEXT("TargetActor"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName IsInAttackRangeKey = FName(TEXT("IsInAttackRange"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName IsPlayerDetectedKey = FName(TEXT("IsPlayerDetected"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName TargetDistanceKey = FName(TEXT("TargetDistance"));

};
