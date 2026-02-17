#include "Actor/BossBattleArena.h"
#include "Components/BoxComponent.h"
#include "Actor/BossDoor.h"
#include "Actor/MinionSpawner.h"
#include "Character/Boss/Boss.h"


ABossBattleArena::ABossBattleArena()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABossBattleArena::BeginPlay()
{
	Super::BeginPlay();

	if (BossActor)
	{
		BossActor->OnLastAttackStarted.AddDynamic(this, &ABossBattleArena::ClearMinionSpawner);
	}
}

void ABossBattleArena::StartBattle()
{
	bBattleActivated = true;

	//1. Door Close
	if (ArenaDoor)
	{
		ArenaDoor->CloseDoor();  
	}

	//2. Start BattleStart Motion
	if (BossActor)
	{
		BossActor->OnBattleTriggered();
	}
}

//set with blueprint node
void ABossBattleArena::InitializeCollisionBox(UBoxComponent* CollisionBox)
{
	BattleTrigger = CollisionBox;
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

	// Linear Interpolation)
	float Alpha = FMath::Min(DoorAnimationElapsedTime / DoorAnimationDuration, 1.0f);

	FVector StartPosition = ArenaDoor->GetActorLocation();
	FVector NewLocation = FMath::Lerp(StartPosition, ClosedDoorPosition, Alpha);

	ArenaDoor->SetActorLocation(NewLocation);

	// Anim finished
	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(DoorTimer);
	}
}

