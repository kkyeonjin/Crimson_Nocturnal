// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Boss/BossAttributeComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Component/Player/PlayerAttributeComponent.h"
#include "Character/Boss/Boss.h"
#include "Character/Boss/Types/BossTypes.h"
#include "Character/Boss/Types/BossStat.h"

// Sets default values for this component's properties
UBossAttributeComponent::UBossAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBossAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter)
	{
		PlayerAttributeComponent = PlayerCharacter->FindComponentByClass<UPlayerAttributeComponent>();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter From EnemyComp Load Failed"));
	}

	FBossStat* Row = BossStatTable->FindRow<FBossStat>(TEXT("Boss"), TEXT("Load Boss Stat Data"));
	if (Row)
	{
		DEF = Row->DEF;
		CurrentHP = Row->CurrentHP;
		MaxHP = Row->MaxHP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BossStatTable Load Failed"));
	}
	// ...

	//for Debug
	//OnHpThreshold.Broadcast(1);

}

int32 UBossAttributeComponent::CalcDamage()
{
	//set ATK after checking the current phase
	//SetPhaseNum();

	if (!PlayerAttributeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("BAC is not Found"));
		return 0;
	}
	int32 BaseDamage = ATK - PlayerAttributeComponent->GetDEF();
	int32 FinalDamage = FMath::Max(BaseDamage, 0);

	return FinalDamage;

}

void UBossAttributeComponent::ApplyDamage()
{
	if (PlayerAttributeComponent)
	{
		int32 Damage = PlayerAttributeComponent->CalcDamage();

		if (IsInfected)
		{
			Damage *= 2;
		}

		CurrentHP -= Damage;
		UE_LOG(LogTemp, Display, TEXT("BossHP: %d"), CurrentHP);

		if (CurrentHP < HP_THRESHOLD_2 )
		{
			OnHpThreshold.Broadcast(2);
		}

		if (CurrentHP < HP_THRESHOLD_1 )
		{
			//Å©¸®½ºÅ» µ¨¸®°ÔÀÌÆ®
			OnHpThreshold.Broadcast(1);
		}


	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PAC CANNOT FIND in BAC"));
	}
}

