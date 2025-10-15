// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyComponents/EnemyAttributeComponent.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerComponents/PlayerAttributeComponent.h"

// Sets default values for this component's properties
UEnemyAttributeComponent::UEnemyAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyAttributeComponent::BeginPlay()
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

	FEnemyStat* Row = EnemyStatTable->FindRow<FEnemyStat>(FName(TEXT("Mutant")), TEXT("Load Enemy Stat Data"));
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
	// ...
	
}


void UEnemyAttributeComponent::InitializeEnemyStat()
{
}



int32 UEnemyAttributeComponent::CalcDamage()
{
	if (!PlayerAttributeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("EAC is not Found"));
		return 0.f;
	}
	float Damage = ATK - PlayerAttributeComponent->GetDEF();
	if (Damage > 0)
	{
		return FMath::RoundToInt32(Damage);
	}
	else
	{
		return 0;
	}
}

void UEnemyAttributeComponent::ApplyDamage()
{
	if (PlayerAttributeComponent)
	{
		int32 Damage = PlayerAttributeComponent->CalcDamage();
		CurrentHP -= Damage;
		UE_LOG(LogTemp, Display, TEXT("EnemyHP: %d"), CurrentHP);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PAC CANNOT FIND"));
	}
}

void UEnemyAttributeComponent::SendEXPValue()
{
	PlayerAttributeComponent->AddEXP(EXPValue);
}

