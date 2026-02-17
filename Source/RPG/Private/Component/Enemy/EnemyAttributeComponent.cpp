// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Enemy/EnemyAttributeComponent.h"
#include "Character/Enemy/Enemy.h"
#include "Character/Enemy/Types/EnemyStat.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Component/Player/PlayerAttributeComponent.h"

UEnemyAttributeComponent::UEnemyAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UEnemyAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Cache PlayerAttribute safely
	CachePlayerAttribute();

	// Load stats from DataTable
	if (AEnemy* OwnerEnemy = Cast<AEnemy>(GetOwner()))
	{
		EnemyTypeName = OwnerEnemy->GetEnemyTypeName();
	}

	FEnemyStat* Row = EnemyStatTable->FindRow<FEnemyStat>(FName(EnemyTypeName), TEXT("Load Enemy Stat Data"));
	if (Row)
	{
		ATK = Row->ATK;
		DEF = Row->DEF;
		CurrentHP = Row->CurrentHP;
		EXPValue = Row->EXPValue;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyStatTable Load Failed"));
	}
}

void UEnemyAttributeComponent::CachePlayerAttribute()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		CachedPlayerAttributeComponent = Player->FindComponentByClass<UPlayerAttributeComponent>();
	}
}

int32 UEnemyAttributeComponent::CalcDamage()
{
	if (!CachedPlayerAttributeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerAttributeComp is not Found"));
		return 0;
	}

	int32 BaseDamage = ATK - CachedPlayerAttributeComponent->GetDEF();
	int32 FinalDamage = FMath::Max(BaseDamage, 0);

	return FinalDamage;
}

void UEnemyAttributeComponent::ApplyDamage()
{
	if (!CachedPlayerAttributeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No PlayerAttribute cached"));
		return;
	}

	int32 Damage = CachedPlayerAttributeComponent->CalcDamage();
	if (IsInfected)
	{
		Damage *= 2;
	}

	CurrentHP -= Damage;
	UE_LOG(LogTemp, Display, TEXT("EnemyHP: %d"), CurrentHP);
}

void UEnemyAttributeComponent::SendEXPValue()
{
	if (CachedPlayerAttributeComponent)
	{
		CachedPlayerAttributeComponent->AddEXP(EXPValue);
		UE_LOG(LogTemp, Log, TEXT("EXP granted: %d"), EXPValue);
	}
}
