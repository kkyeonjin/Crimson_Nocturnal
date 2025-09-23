// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_Attack.h"
#include "AIController.h"
#include "Enemy.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attack");

}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    UE_LOG(LogTemp, Display, TEXT("ExecuteAttack"));

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("OwnerComp.GetAIOwner() is nullptr"));
        return EBTNodeResult::Failed;
    }

    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (AIPawn == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Pawn is nullptr"));
        return EBTNodeResult::Failed;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Pawn Class: %s"), *AIPawn->GetClass()->GetName());
    }

    AEnemy* EnemyLocal = Cast<AEnemy>(AIPawn);
    if (EnemyLocal == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Cast to AEnemy failed"));
        return EBTNodeResult::Failed;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Cast succeeded"));
    }

    UE_LOG(LogTemp, Display, TEXT("Calling AttackStart..."));
    EnemyLocal->AttackStart();

    UE_LOG(LogTemp, Display, TEXT("AttackStart called"));

    return EBTNodeResult::InProgress;
}
