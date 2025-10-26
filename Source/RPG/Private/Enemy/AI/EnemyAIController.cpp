// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/AI/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Owner && Owner->GetEnemyCombatState() == EEnemyCombatState::EECS_Dead)
	{
		StopMovement();
		UCharacterMovementComponent* MoveComp = Owner->GetCharacterMovement();
		if (MoveComp)
		{
			MoveComp->StopMovementImmediately();
			MoveComp->DisableMovement();
		}
		return;
	}
}

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
		UE_LOG(LogTemp, Warning, TEXT("Perception Set"));
	}

	Owner = Cast<AEnemy>(GetPawn());

	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
}

//When Enemy detects player 
void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogTemp, Warning, TEXT("PerceptionUpdated"));
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