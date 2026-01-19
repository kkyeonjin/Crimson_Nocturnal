// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BossBattleArena.h"
#include "Components/BoxComponent.h"
#include "Actor/MinionSpawner.h"
#include "Character/Boss/Boss.h"

// Sets default values
ABossBattleArena::ABossBattleArena()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossBattleArena::BeginPlay()
{
	Super::BeginPlay();

	if (BossActor)
	{
		BossActor->OnLastAttackStarted.AddDynamic(this, &ABossBattleArena::ClearMinionSpawner);
		UE_LOG(LogTemp, Display, TEXT("ClearMinionDelegateBind"));
	}

}

void ABossBattleArena::StartBattle()
{
	bBattleActivated = true;

	//1. Door Close
	if (ArenaDoor)
	{
		UE_LOG(LogTemp, Warning, TEXT("DoorChecked"));

		GetWorld()->GetTimerManager().SetTimer(
			DoorTimer,
			this,
			&ABossBattleArena::CloseDoor,
			DoorCloseTime,
			true
		);
	}

	//2. Start BattleStart Motion
	if (BossActor)
	{
		BossActor->OnBattleTriggered();
	}

	//OnBattleTriggered.Broadcast();
}

//set with blueprint node
void ABossBattleArena::InitializeCollisionBox(UBoxComponent* CollisionBox)
{
	UE_LOG(LogTemp, Display, TEXT("CollisionInitialize"));

	BattleTrigger = CollisionBox;

	//Collision Binding
	BattleTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABossBattleArena::OnBattleTriggerOverlap);
}

//callback func called by boss(when QTE starts)
void ABossBattleArena::ClearMinionSpawner()
{
	CachedMinionSpawner->ClearAllMinions();
}

void ABossBattleArena::OnBattleTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Only When the Battle has not started yet
	if (OtherActor->ActorHasTag(TEXT("Player")) && !bBattleActivated)
	{
		StartBattle();

		GetWorld()->GetTimerManager().SetTimer(
			DoorTimer,
			this,
			&ABossBattleArena::SpawnMinionSpawner,
			5.f,
			false
		);
	}
}

void ABossBattleArena::SpawnMinionSpawner()
{
	if (MinionSpawnerClass && GetWorld())
	{
		CachedMinionSpawner = GetWorld()->SpawnActor<AMinionSpawner>(
			MinionSpawnerClass,
			GetActorLocation(),
			FRotator::ZeroRotator);
	}
}

void ABossBattleArena::CloseDoor()
{
	DoorAnimationElapsedTime += DoorCloseTime;

	// 선형 보간 (Linear Interpolation)
	float Alpha = FMath::Min(DoorAnimationElapsedTime / DoorAnimationDuration, 1.0f);

	FVector StartPosition = ArenaDoor->GetActorLocation();
	//FVector TargetPosition = bIsOpen ? OpenPosition : ClosedPosition;
	FVector NewLocation = FMath::Lerp(StartPosition, ClosedDoorPosition, Alpha);

	ArenaDoor->SetActorLocation(NewLocation);

	// 애니메이션 완료
	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(DoorTimer);
	}

}

