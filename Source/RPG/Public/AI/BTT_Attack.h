// Attack Node for enemy AI
// Called when distance between player&enemy getting closer than AttackRadius

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

class AEnemy;

UCLASS()
class RPG_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnEnemyAttackEnd();

private:
	AEnemy* Enemy;
	UBehaviorTreeComponent* CachedOwnerComp;

};
