// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTService_UpdateDistance.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

void UBTService_UpdateDistance::TickNode(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory,float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    AAIController* AIController = OwnerComp.GetAIOwner();

    // ← Blueprint의 "Is Valid" 같은 부분
    if (!Blackboard || !AIController)
        return;

    // ← "Get Blackboard Value as Actor" 같은 부분
    AActor* TargetActor = Cast<AActor>(Blackboard->GetValue<UBlackboardKeyType_Object>(Blackboard->GetKeyID(TargetActorKey)));

    APawn* EnemyPawn = AIController->GetPawn();

    // ← 다시 "Is Valid" 체크
    if (!TargetActor || !EnemyPawn)
        return;

    // ← "Get Actor Location" (Enemy)
    FVector EnemyLocation = EnemyPawn->GetActorLocation();

    // ← "Get Actor Location" (Player)
    FVector TargetLocation = TargetActor->GetActorLocation();

    // ← "Distance (Vector)"
    float Distance = FVector::Dist(EnemyLocation, TargetLocation);

    // ← ">= 비교"
    bool bIsInRange = Distance <= AttackRadius;  // 주의: <= 임!

    // ← "Set Blackboard Value as Bool" (IsInAttackRange)
    Blackboard->SetValue<UBlackboardKeyType_Bool>(Blackboard->GetKeyID(IsInAttackRangeKey),bIsInRange);

    if (Distance >= 1500.f)
    {
        Blackboard->SetValue<UBlackboardKeyType_Bool>(Blackboard->GetKeyID(IsPlayerDetectedKey), false);
    }

    // ← "Set Blackboard Value as Bool" (IsPlayerDetected)
    //Blackboard->SetValue<UBlackboardKeyType_Bool>(Blackboard->GetKeyID(IsPlayerDetectedKey),true);
}