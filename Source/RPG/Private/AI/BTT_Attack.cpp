#include "AI/BTT_Attack.h"
#include "AIController.h"
#include "Character/Enemy/Enemy.h"

UBTT_Attack::UBTT_Attack()
{
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    CachedOwnerComp = &OwnerComp;

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (AIPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AEnemy* EnemyLocal = Cast<AEnemy>(AIPawn);
    if (EnemyLocal == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    // Hand over BT & Node to Enemy
    // Manages ATTACK state entry only - actual animation playback & attack logic handled individually within Enemy class!
	// Prevents simultaneous attacks across multiple enemies
	// Ensures independent behavior for each enemy instance
    EnemyLocal->AttackStart(&OwnerComp, this);

    return EBTNodeResult::InProgress;
}

void UBTT_Attack::OnEnemyAttackEnd()
{
    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

