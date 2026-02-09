// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BossUltimateSkill.h"
#include "Character/Player/PlayerCharacter.h"

// Sets default values
ABossUltimateSkill::ABossUltimateSkill()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ABossUltimateSkill::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		StormTimer,
		this,
		&ABossUltimateSkill::StartUltSkill,
		1.f,
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
			GetActorLocation(), //임시로 넣은 값
			FRotator::ZeroRotator);
	}

	TeleportPlayerToCenter();
}


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

	UE_LOG(LogTemp, Warning,
		TEXT("UltimateSkillActor: Player teleported to center at %.0f, %.0f, %.0f"),
		TeleportLocation.X, TeleportLocation.Y, TeleportLocation.Z);
}

