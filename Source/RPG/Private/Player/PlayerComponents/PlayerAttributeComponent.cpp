// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerComponents/PlayerAttributeComponent.h"
#include "Enemy/EnemyComponents/EnemyAttributeComponent.h"

// Sets default values for this component's properties
UPlayerAttributeComponent::UPlayerAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	//get Stats from DataTable to variables
	FPlayerStat* Row = PlayerStatTable->FindRow<FPlayerStat>(FName(TEXT("MainPlayer")), TEXT("Load Player Stat Data"));
	if (Row)
	{
		ATK = Row->ATK;
		DEF = Row->DEF;
		CurrentHP = Row->CurrentHP;
		CurrentBP = Row->CurrentBp;
		EXP = Row->EXP;
		Level = Row->Level;
		SkillPoint = Row->SkillPoint;

	}
	// ...
	
}

void UPlayerAttributeComponent::InitializePlayerStat()
{
	//PlayerStat.
}

//Calculation Damage Value to Enemy when Player hits Enemy
int32 UPlayerAttributeComponent::CalcDamage()
{
	if (BPtoGet != 0)
	{
		UE_LOG(LogTemp, Display, TEXT("BPtoGet: %d"), BPtoGet);
		CurrentBP += BPtoGet;
		//CurrentBP = FMath::Clamp(CurrentBP, 0.f, MaxBP);
		UE_LOG(LogTemp, Display, TEXT("CurrentBP: %d"), CurrentBP);
		BPtoGet = 0;
	}

	if (!EnemyAttributeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("EAC is not Found"));
		return 0.f;
	}
	
	float Damage = ATK - EnemyAttributeComponent->GetDEF();
	if (Damage > 0)
	{
		return FMath::RoundToInt32(Damage);
	}
	else
	{
		return 0;
	}
}

//Apply Damage(=Reduce Player's HP) that Enemy gave to Player
void UPlayerAttributeComponent::ApplyDamage()
{
	if (EnemyAttributeComponent)
	{
		int32 Damage = EnemyAttributeComponent->CalcDamage();
		CurrentHP -= Damage;
		UE_LOG(LogTemp, Display, TEXT("PlayerHP: %d"), CurrentHP);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("EnemyAttributeComponent CANNOT FIND"));
	}
}

void UPlayerAttributeComponent::SetEnemyAttributeComponent(AActor* Target)
{
	EnemyAttributeComponent = Target->FindComponentByClass<UEnemyAttributeComponent>();
	if (!EnemyAttributeComponent)
	{
		//UE_LOG(LogTemp, Display, TEXT("EnemyAttributeComp Cannot Find"));
	}
}

//take Enemy's EXP value who player just killed 
void UPlayerAttributeComponent::AddEXP(int32 EXPValue)
{
	EXP += EXPValue;

	while (EXP >= GetRequiredEXPForNextLevel())
	{
		EXP -= GetRequiredEXPForNextLevel();
		Level++;

		// OnLevelUp(); // 레벨 상승 시 추가 이벤트 많이 필요하면 호출
		SkillPoint += 2;
	}

	UE_LOG(LogTemp, Display, TEXT("EXP: %d"), EXP);
}

