// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MinionSpawner.h"
#include "Character/Enemy/Enemy.h"

// Sets default values
AMinionSpawner::AMinionSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AMinionSpawner::BeginPlay()
{
	Super::BeginPlay();


	for (int i = 0; i < MaxMinions; i++)
	{
		SpawnPointsIndex++;
		SpawnNewMinion();
	}

	SpawnPointsIndex = 0;
	//MonsterNum = 3;

}

void AMinionSpawner::SpawnNewMinion()
{
	if (GetWorld())
	{
		UE_LOG(LogTemp, Display, TEXT("MinionSpawn,,,,"));

		AEnemy* NewMinion = GetWorld()->SpawnActor<AEnemy>(
			Monster,
			SpawnPoints[SpawnPointsIndex],
			FRotator::ZeroRotator);

		if (NewMinion)
		{
			NewMinion->SetOwner(this);

			IDefeatInterface* DefeatInterface = Cast<IDefeatInterface>(NewMinion);

			if (DefeatInterface)
			{
				FOnCharacterDefeated& DefeatedDelegate = DefeatInterface->OnCharacterDefeatedDelegate();

				DefeatedDelegate.AddDynamic(this, &AMinionSpawner::OnMinionDefeated);
			}

			AliveMinions.Add(NewMinion);
		}
	}

}

void AMinionSpawner::OnMinionDefeated(AActor* Minion)
{

	AEnemy* DefeatedEnemy = Cast<AEnemy>(Minion);
	if (DefeatedEnemy)
	{
		AliveMinions.Remove(DefeatedEnemy);  // 배열에서 제거
	}


	SpawnPointsIndex = FMath::Rand() % 3;

	//Timer로 지연
	// 페이드아웃 타이머 시작 (0.05초마다 업데이트)
	GetWorldTimerManager().SetTimer(
		RespawnTimer,
		this,
		&AMinionSpawner::SpawnNewMinion,
		5.f,  // 업데이트 간격
		false    // 반복
	);

}

void AMinionSpawner::ClearAllMinions()
{
	UE_LOG(LogTemp, Warning, TEXT("ClearAllMinions called, AliveMinions count: %d"), AliveMinions.Num());

	for (int32 i = AliveMinions.Num() - 1; i >= 0; --i)
	{
		if (AliveMinions[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroying minion at index %d"), i);
			AliveMinions[i]->Destroy();
		}
	}

	AliveMinions.Empty();

	UE_LOG(LogTemp, Warning, TEXT("ClearAllMinions completed"));
}

