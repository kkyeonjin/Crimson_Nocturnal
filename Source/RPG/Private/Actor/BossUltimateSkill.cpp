#include "Actor/BossUltimateSkill.h"
#include "Character/Player/PlayerCharacter.h"


ABossUltimateSkill::ABossUltimateSkill()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABossUltimateSkill::BeginPlay()
{
	Super::BeginPlay();


	// Delayed spawn 
	// Storm actor(UltActor) must be spawned after the Boss Ult Anim when finished
	GetWorld()->GetTimerManager().SetTimer(
		StormTimer,
		this,
		&ABossUltimateSkill::StartUltSkill,
		0.5f,
		false
	);
}

void ABossUltimateSkill::StartUltSkill()
{
	//Spawn Storm Actor
	if (GetWorld())
	{
		GetWorld()->SpawnActor<AActor>(
			DarkStormActorClass,
			GetActorLocation(), 
			FRotator::ZeroRotator);
	}

	TeleportPlayerToCenter();
}

// Teleport Player to UltActor's location (trapped)
void ABossUltimateSkill::TeleportPlayerToCenter()
{
	if (!TargetPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("UltimateSkillActor: Target player not set!"));
		return;
	}

	FVector CenterLocation = GetActorLocation();
	FVector TeleportLocation = CenterLocation + FVector(0, 0, TeleportHeight);

	TargetPlayer->SetActorLocation(TeleportLocation);
	TargetPlayer->StartBossQTEMode();
}

