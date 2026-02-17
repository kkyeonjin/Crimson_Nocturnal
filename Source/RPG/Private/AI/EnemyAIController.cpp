#include "AI/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/Enemy.h"


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	if (!Blackboard)
	{
		Blackboard = GetBlackboardComponent();
	}
	if (!PerceptionComponent)
	{
		PerceptionComponent = GetPerceptionComponent();
	}

	Owner = Cast<AEnemy>(GetPawn());

	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
}

//When Enemy detects player 
void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			FActorPerceptionBlueprintInfo Info;
			PerceptionComponent->GetActorsPerception(Actor, Info);

			bool IsDetected = false;

			//use reference to avoid unnecessary copy
			for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
			{
				if (Stimulus.WasSuccessfullySensed())
				{
					IsDetected = true;
					break;
				}
			}

			if (IsDetected)
			{
				TargetActor = Actor;
				Blackboard->SetValueAsObject("TargetActor", TargetActor);
				Blackboard->SetValueAsBool("IsPlayerDetected", true);

			}
			else
			{
				Blackboard->ClearValue("TargetActor");
				Blackboard->SetValueAsBool("IsPlayerDetected", false);
				TargetActor = nullptr;
			}
		}
	}
}

