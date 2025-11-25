// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/AI/BTT_Attack.h"
#include "AIController.h"
#include "Enemy/Enemy.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    CachedOwnerComp = &OwnerComp;

    //UE_LOG(LogTemp, Display, TEXT("ExecuteAttack"));

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        //UE_LOG(LogTemp, Error, TEXT("OwnerComp.GetAIOwner() is nullptr"));
        return EBTNodeResult::Failed;
    }

    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (AIPawn == nullptr)
    {
        //UE_LOG(LogTemp, Error, TEXT("Pawn is nullptr"));
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
        //UE_LOG(LogTemp, Display, TEXT("Cast succeeded"));
    }

    //UE_LOG(LogTemp, Display, TEXT("Calling AttackStart..."));
    EnemyLocal->AttackStart(&OwnerComp, this);

    //UE_LOG(LogTemp, Display, TEXT("[%s] Attack Task Started"), *EnemyLocal->GetName());

    //UE_LOG(LogTemp, Display, TEXT("AttackStart called"));
   // EnemyLocal->OnEnemyAttackEnd.RemoveDynamic(this, &UBTT_Attack::OnEnemyAttackEnd);
    //EnemyLocal->OnEnemyAttackEnd.AddDynamic(this, &UBTT_Attack::OnEnemyAttackEnd);

    return EBTNodeResult::InProgress;
}

void UBTT_Attack::OnEnemyAttackEnd()
{
    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

