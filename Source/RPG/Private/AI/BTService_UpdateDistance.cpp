#include "AI/BTService_UpdateDistance.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

void UBTService_UpdateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    AAIController* AIController = OwnerComp.GetAIOwner();

    if (!Blackboard || !AIController)
        return;

    AActor* TargetActor = Cast<AActor>(Blackboard->GetValue<UBlackboardKeyType_Object>(Blackboard->GetKeyID(TargetActorKey)));
    APawn* EnemyPawn = AIController->GetPawn();

    if (!TargetActor || !EnemyPawn)
        return;

    FVector EnemyLocation = EnemyPawn->GetActorLocation();
    FVector TargetLocation = TargetActor->GetActorLocation();

    float Distance = FVector::Dist(EnemyLocation, TargetLocation);

    bool bIsInRange = Distance <= AttackRadius;  

    // ก็ "Set Blackboard Value as Bool" (IsInAttackRange)
    Blackboard->SetValue<UBlackboardKeyType_Bool>(Blackboard->GetKeyID(IsInAttackRangeKey), bIsInRange);

    /*
    // Set IsPlayerDetected -> false when distance gets longer 
    // Enable to start patrol again
    if (Distance >= 1500.f)
    {
        Blackboard->SetValue<UBlackboardKeyType_Bool>(Blackboard->GetKeyID(IsPlayerDetectedKey), false);
    }
	*/
}

