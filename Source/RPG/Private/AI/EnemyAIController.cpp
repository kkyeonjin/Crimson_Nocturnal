// Fill out your copyright notice in the Description page of Project Settings.
// TargetActor 초기 설정 해주는 정도의 역할?? 

#include "AI/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

//이 상태 관리는 enemy로 옮겨서 각 객체에서 관리하도록 -> Tick은 안 쓰도록 구조 변경!!!
void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	/*
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
	*/
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
		//UE_LOG(LogTemp, Warning, TEXT("Perception Set"));
	}

	Owner = Cast<AEnemy>(GetPawn());

	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
	//PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

//When Enemy detects player 
void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	//UE_LOG(LogTemp, Warning, TEXT("PerceptionUpdated"));
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

